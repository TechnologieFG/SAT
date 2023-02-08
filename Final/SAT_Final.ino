//TP & PA  Librerías y configuraciones =======================
#include <SFE_BMP180.h>
#include <Wire.h>

SFE_BMP180 bmp180;

//HD DHT11 Librerías y configuraciones =======================
#include <DHT.h>    // importa la Librerias DHT
#include <DHT_U.h>

//SIM900 Librerías y configuraciones =======================
#include <SoftwareSerial.h>
SoftwareSerial sim(7, 8); //Seleccionamos los pines 7 (10 del MEGA) como Rx y 8 (11 del MEGA) como Tx

//NC Constantes y puertos============================================= 
const int SNC1 = 3;     // SENSOR DE CAUSE 1
const int SNC2 = 4;     // SENSOR DE CAUSE 2
const int SNC3 = 5;     // SENSOR DE CAUSE 3
const int SNC4 = 10;     // SENSOR DE CAUSE 4
const int SNC5 = 11;     // SENSOR DE CAUSE 5
//VC Constantes y puertos=============================================
int SVC = 2;    //Sensor conectado en el pin 2
int TiempoMuestreo=2000; 
//TP & PA PMB180 Constantes y puertos=============================================

//HD DHT11 Constantes y puertos=============================================
int SENSOR = 12;     // pin DATA de DHT22 a pin digital 10
//int TEMPERATURA;
int HUMEDAD;
DHT dht(SENSOR, DHT11);   // creacion del objeto, cambiar segundo parametro

//VV YGC-FS-KV-A1 Constantes y puertos=============================================
const int SVVPin=A0;
float A = 45;
float B = 0;
float ajuste=0.55;

//NC Variables============================================= 
int SSNC1 = 0;         // SIGNAL SENSOR DE CAUSE 1
int SSNC2 = 0;         // SIGNAL SENSOR DE CAUSE 2
int SSNC3 = 0;         // SIGNAL SENSOR DE CAUSE 3
int SSNC4 = 0;         // SIGNAL SENSOR DE CAUSE 4
int SSNC5 = 0;         // SIGNAL SENSOR DE CAUSE 5
String NC="0000";
//VC Variables=============================================
volatile int NumPulsos; //variable para la cantidad de pulsos recibidos
float factor_conversion=7.5; //para convertir de frecuencia a caudal
float caudal_L_m=0;
float frecuencia=0;
//TP & PA BMP180 Variables=============================================
char status;
double T,P; 
//Anemómetro Variables=============================================
float VoltageValue = 0;
float SpeedValue = 0;
float SumSpeedValue = 0;
int Contador = 0;
float VV=0;

// SendaData Variables============================================
int NPrueba=0; //variable para la cantidad de pulsos recibidos


void setup() {
  Serial.begin(19200);   // inicializacion de monitor serial
  //NC Setup ===================
  pinMode(SNC1, INPUT);
  pinMode(SNC2, INPUT);
  pinMode(SNC3, INPUT);
  pinMode(SNC4, INPUT);
  pinMode(SNC5, INPUT);
//NC Setup ===================
  pinMode(SVC, INPUT); 
  attachInterrupt(0,ContarPulsos,RISING); //(Interrupcion 0(Pin2),funcion,Flanco de subida)
//TP&PA BMP180 Setup ===================  
  bmp180.begin();
  /*
  if (bmp180.begin())
    Serial.println("BMP180 iniciado correctamenten");
  else
  {
    Serial.println("Error al iniciar el BMP180");
    while(1); // bucle infinito
  }
  */
  //HD DHT11 Setup ===================  
  dht.begin();      // inicializacion de sensor
  //SIM900 Setup ===================  
  sim.begin(19200);
  delay(1000);
}

void loop() {
  FNC();
  VC();
  TP_PA();
  HD();
  FVV();
  SendData();
  if (Serial.available() > 0)
  sim.write(Serial.read());
 
  if (sim.available() > 0)
  Serial.write(sim.read());
}
// Funciones
//NC Función ===============================================
void FNC(){
  SSNC1 = digitalRead(SNC1);
  SSNC2 = digitalRead(SNC2);
  SSNC3 = digitalRead(SNC3);
  SSNC4 = digitalRead(SNC4);
  SSNC5 = digitalRead(SNC5);

  if (SSNC1 == HIGH && SSNC2 == HIGH && SSNC3 == HIGH && SSNC4 == HIGH && SSNC5 == HIGH) {
    NC="0000";
  }
  else if (SSNC1 == LOW && SSNC2 == HIGH && SSNC3 == HIGH && SSNC4 == HIGH && SSNC5 == HIGH) {
    NC="0020";
  }
  else if (SSNC1 == HIGH && SSNC2 == LOW && SSNC3 == HIGH && SSNC4 == HIGH && SSNC5 == HIGH) {
    NC="0040";
  }
  else if (SSNC1 == HIGH && SSNC2 == HIGH && SSNC3 == LOW && SSNC4 == HIGH && SSNC5 == HIGH) {
    NC="0060";
  }
  else if (SSNC1 == HIGH && SSNC2 == HIGH && SSNC3 == HIGH && SSNC4 == LOW && SSNC5 == HIGH) {
    NC="0080";
  }
  else if (SSNC1 == HIGH && SSNC2 == HIGH && SSNC3 == HIGH && SSNC4 == HIGH && SSNC5 == LOW) {
    NC="0100";
  }
}

//VC Funciónes ===============================================

//---Función que se ejecuta en interrupción---------------
void ContarPulsos ()
{ 
  NumPulsos++;  //incrementamos la variable de pulsos
} 

//---Función para obtener frecuencia de los pulsos--------
int ObtenerFrecuencia() 
{
  int frecuencia;
  NumPulsos = 0;   //Ponemos a 0 el número de pulsos
  interrupts();    //Habilitamos las interrupciones
  delay(TiempoMuestreo);   //muestra de 1 segundo
  //noInterrupts(); //Desabilitamos las interrupciones
  //La línea de codigo anterior no permitía trabajar en conjunto con el BMP
  frecuencia=NumPulsos/(TiempoMuestreo/1000); //Hz(pulsos por segundo)
  return frecuencia;
}

void VC(){
  frecuencia=ObtenerFrecuencia(); //obtenemos la Frecuencia de los pulsos en Hz
  caudal_L_m=frecuencia/factor_conversion; //calculamos el caudal en L/m
  //float caudal_L_h=caudal_L_m*60; //calculamos el caudal en L/h
}

//TP & PB BMP180 Funciónes ===============================================


void TP_PA(){
  status = bmp180.startTemperature();//Inicio de lectura de temperatura
  if (status != 0)
  {   
    delay(status); //Pausa para que finalice la lectura
    status = bmp180.getTemperature(T); //Obtener la temperatura
    if (status != 0)
    {
      status = bmp180.startPressure(3); //Inicio lectura de presión
      if (status != 0)
      {        
        delay(status);//Pausa para que finalice la lectura        
        status = bmp180.getPressure(P,T); //Obtenemos la presión
      }
    }
  }
}

void HD(){
  //TEMPERATURA = dht.readTemperature();  // obtencion de valor de temperatura
  HUMEDAD = dht.readHumidity();   // obtencion de valor de humedad
}

void FVV(){
  float sensorValue = analogRead(SVVPin);
  //Serial.println(sensorValue);
  //VoltageValue = sensorValue*5/1023;
  //Serial.println(VoltageValue);
  VoltageValue = sensorValue*5/1023-ajuste;
  //Serial.println(VoltageValue);
  SpeedValue = abs((VoltageValue-0.835)/4.165*A+B);
}

void SendData(){
  NPrueba++;
  //Prueba “n”
  //Estacion remota CP 0035 NC 0002 VC 0011 TP23.6 PA 1028 HD 30.8 VV 000110100110
  //Mensaje enviado

    //-----Enviamos por el puerto serie---------------
  Serial.print ("Prueba "); 
  Serial.println (NPrueba); 
  Serial.println (" "); 
  Serial.print ("EstacionRemotaNC");
  Serial.print (NC);
  Serial.print ("VC");
  Serial.print (caudal_L_m);
  Serial.print ("TP");
  Serial.print (T);
  Serial.print ("PA");
  Serial.print (P);
  Serial.print ("HD");
  Serial.print (HUMEDAD);
  Serial.print ("VV");
  Serial.println (SpeedValue);
  Serial.println (" ");
}

