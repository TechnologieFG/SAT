#include <SoftwareSerial.h>
SoftwareSerial sim(7, 8); //Seleccionamos los pines 7 (10 del MEGA) como Rx y 8 (11 del MEGA) como Tx

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 9;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

int _timeout;
String _buffer;
String number1 = "+525568780924";
String number2 = "+525561713009";
String number3 = "+525586864746";
String VolumenNivel = "25";
int SendFlag =1;


void setup()
{
  sim.begin(19200);
  Serial.begin(19200);
  delay(1000);
   
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
 // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  //Vol();
  delay(100);
  
  // sim.println("AT+SAPBR=0,1");
  // delay(100);
  //sim.println("AT+CMMSTERM \r");
  //delay(100);
  
}

void loop()
{ 
  if (Serial.available() > 0)
  sim.write(Serial.read());
 
  if (sim.available() > 0)
  Serial.write(sim.read());
  //Button();
  //sendMMS();
  //Call();  
  //Sms();
  FRead();
}

void FRead(){
  if (SendFlag==1){
    sim.println("AT+CSCS=\"GSM\"");
    delay(100);
    sim.println("AT+CMGR=2");
    
    // sim.println("AT+CMGR=3");
    // delay(100);
  // AT+CMGR=2
  // AT+CMGR=3
  
  // AT+CMGD=1
  // AT+CMGD=2
  // AT+CMGD=3
    // sim.println("AT+CMGD=1");
    // delay(100);
    // sim.println("AT+CMGD=2");
    // delay(100);
    // sim.println("AT+CMGD=3");
    // delay(100);
  SendFlag=0;
  }
  else if(SendFlag==2){
    SendFlag=0;
    sim.println("AT+CMGR=2");
  }
  
}

void Button(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin); 
}

void Call(){//Envíamos y recibimos datos

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);// turn LED on:
    sim.println("AT+STTONE=1,1,2000");
    delay(3000);
    sim.println("ATD" + number2 + ";");
    delay(1000);    
  } else {
    digitalWrite(ledPin, LOW);// turn LED off:
  }
}

void Sms()
{
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);// turn LED on:
    sim.println("AT+STTONE=1,1,2000");
    delay(3000);
    //SendMessage();
    //delay(1000);    
    
    //Serial.println ("Sending Message");
    sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);
    //Serial.println ("Set SMS numbee1");
    sim.println("AT+CMGS=\"" + number2 + "\"\r"); //Mobile phone number to send message
    delay(1000);
    String SMS = "Hallo, Wie geht's?";
    sim.println(SMS);
    delay(100);
    sim.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
    _buffer = _readSerial();
  } else {
    digitalWrite(ledPin, LOW);// turn LED off:
  }
}

String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}

void Vol(){
  sim.println("AT+CLVL=" + VolumenNivel + "\r"); //Mobile phone numbee1 to send message  
  }

void sendMMS(){
  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);// turn LED on:

    //Initialization
    
    sim.print("AT+CMMSINIT\r");
    delay(1000);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }

    //Configuration
    
    sim.print("AT+CMMSCURL=\"http://mms.itelcel.com/servlets/mms\"\r");
    delay(100);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    sim.print("AT+CMMSCID=1\r");
    delay(100);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    sim.print("AT+CMMSPROTO=\"148.233.151.240\",8080\r");
    delay(100);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    sim.print("AT+CMMSSENDCFG=6,3,0,0,2,4\r");
    delay(100);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }

    //Active bearer profile
    
    sim.print("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r");
    delay(100);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    sim.print("AT+SAPBR=3,1,\"APN\",\"mms.itelcel.com\"\r");
    delay(100);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    
    sim.print("AT+SAPBR=3,1,\"USER\",\"mmsgprs\"\r");
    delay(100);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    sim.print("AT+SAPBR=3,1,\"PWD\",\"mmsgprs2003\"\r");
    delay(100);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }

    
    sim.print("AT+SAPBR=1,1\r");
    delay(4000);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    sim.print("AT+SAPBR=2,1\r");
    delay(1000);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }

    sim.print("AT+SAPBR=4,1\r");
    delay(1000);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    
    
    //Send MMS
    
    sim.print("AT+CMMSEDIT=1\r");
    delay(100);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    sim.print("AT+CMMSDOWN=\"TEXT\",5,5000,\"text3.txt\" \r");
    delay(100);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    sim.print("ILOVE \r");
    delay(100);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }

    sim.print("AT+CMMSDOWN=\"TITLE\",3,5000\r");
    delay(100);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    sim.print("HUA \r");
    delay(100);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }

    
    sim.print("AT+CMMSRECP=\"" + number1 + "\"\r");       
    delay(100);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    sim.print("AT+CMMSVIEW \r");
    delay(2000);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    sim.print("AT+CMMSSEND \r");
    delay(2000);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    sim.print("AT+CMMSEDIT=0 \r");
    delay(2000);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    sim.print("AT+CMMSTERM \r");
    delay(2000);
    if(sim.available())
    {
      while(sim.available()) Serial.write(sim.read());
    }
    } else {
    digitalWrite(ledPin, LOW);// turn LED off:
  }


}
