

/////////////////////////////////////////////////////////
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <Arduino_JSON.h>
#include <TinyGPS++.h>
#include <ArduinoJson.h>
//GSM Module TX is connected to Arduino D8
#define SIM800_TX_PIN 7  //7

//GSM Module RX is connected to Arduino D9
#define SIM800_RX_PIN 8//5
SoftwareSerial mySerial(SIM800_TX_PIN, SIM800_RX_PIN); //Create software serial object to communicate with GSM Module
///////////////////////////////////////////////
static const int RXPin = 5, TXPin = 6;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
////////////////////////////////////////////////

String name = "Dustbin1";
String location = "It Manager";

// defines variables
int _timeout;
//int index ;
String number = "";
String message = "";
String mynbr = "+250783159293";
//String mynbr = "+250786903760";
char incomingByte;
String incomingData;
bool atCommand = true;
String _buffer;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////

int j;
int  k;
int l;
int m;
int n;

void setup()
{


  Serial.begin(9600); // Serial Communication for Serial Monitor is starting with 9600 of baudrate speed
  mySerial.begin(9600); // Serial Communication for GSM Module is starting with 9600 of baudrate speed
    ss.begin(4800);
  


///////////////////////////////////////////////////////
// Check if you're currently connected to SIM800L
unsigned long startTime = millis();
while (!mySerial.available() && millis() - startTime < 10000) { // Wait for up to 10 seconds
  mySerial.println("AT");
  delay(1000);
  Serial.println("connecting....");
}

if (mySerial.available()) {
  Serial.println("Connected..");
  mySerial.println("AT+CMGF=1");  //Set SMS Text Mode
  delay(1000);
  mySerial.println("AT+CNMI=1,2,0,0,0");  //procedure, how to receive messages from the network
  delay(1000);
  mySerial.println("AT+CMGL=\"REC UNREAD\""); // Read unread messages
  Serial.println("Ready to receive Commands..");
} else {
  Serial.println("Failed to connect to SIM800L.");
}

 Reply("Welcome","To","Company","You are Amazing!!!");
 delay(100);
 //callNumber();

}

void loop()
{
  //////////////////////////////////////////////////////
  if (mySerial.available()) {
    delay(100);

    // Serial buffer
    while (mySerial.available()) {
      incomingByte = mySerial.read();
      incomingData += incomingByte;
    }

    delay(10);
    if (atCommand == false) {
      receivedMessage(incomingData);
    } else {
      atCommand = false;
    }

    //delete messages to save memory
    if (incomingData.indexOf("OK") == -1) {
      mySerial.println("AT+CMGDA=\"DEL ALL\"");
      delay(1000);
      atCommand = true;
    }

    incomingData = "";
  }

   kora();
    
}

////////////////////////////////////////////////////
void Reply(String text, String text1,String text2,String text3)
{
    mySerial.print("AT+CMGF=1\r");
    delay(1000);
    mySerial.print("AT+CMGS=\""+mynbr+"\"\r");
    delay(1000);
    mySerial.print(text);
       mySerial.print(text1);
          mySerial.print(text2);
           mySerial.print(text3);
    delay(100);
    mySerial.write(0x1A); //ascii code for ctrl-26 //sim800.println((char)26); //ascii code for ctrl-26
    delay(1000);
    Serial.println("SMS Sent Successfully.");
}
//////////////////////////////////////////////////////////////////

void receivedMessage(String inputString) {
unsigned int index;
  //Get The number of the sender
  index = inputString.indexOf('"') + 1;
  inputString = inputString.substring(index);
  index = inputString.indexOf('"');
  number = inputString.substring(0, index);
  Serial.println("Number: " + number);
  //Get The Message of the sender
  index = inputString.indexOf("\n") + 1;
  message = inputString.substring(index);
  message.trim();
  Serial.println("Message: " + message);
  message.toUpperCase(); // uppercase the message received
  delay(50);// Added delay between two readings
}






 
 void kora(){
  
  if(mynbr== number){
  //turn Device 1 ON
  if (message.indexOf("ON") > -1) {

   Serial.println("Location requested");
  ///GPSOK();
  }

  //turn OFF system
  if (message.indexOf("OFF") > -1) {
     Serial.println("System has stop");
  
  }
}
else {
  Serial.println("Please send sms to start");

  }
 }
  String _readSerial() {
  _timeout = 0;
  while  (!mySerial.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (mySerial.available()) {
    return mySerial.readString();
  }
}
  
  
  void callNumber() {
  mySerial.print (F("ATD"));
  mySerial.print (number);
  mySerial.print (F(";\r\n"));
  _buffer = _readSerial();
  Serial.println(_buffer);
}

//////////////////////////////////////////////////////////////////////////////////////
 void GPSOK()
{

     while (ss.available() > 0){
     gps.encode(ss.read());
     if (gps.location.isUpdated()){
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);

      String l= String(gps.location.lat(), 6);
      String t= String(gps.location.lng(), 6);
      
 Reply("Latitude",l,"Longitude",t);
    }
    else{
      Serial.print("GPS IS SEARCHING NETWORK");
      }
  }  
}
