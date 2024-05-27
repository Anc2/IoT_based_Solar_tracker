#define CAYENNE_PRINT Serial
#include <SoftwareSerial.h>

// Redefine the domain BEFORE including the library.
#define CAYENNE_DOMAIN "mqtt.zafron.dev"

#include <CayenneMQTTESP8266.h>

#define rxPin 0 // GPIO0 (labelled IO0 on the ESP01), The actual Rx pin on ESP01 GPIO3,
#define txPin 2 //this is IO2 on the ESP01, the actual Tx pin on the ESP01 is GPIO1
#define pumpVirtualPin 1 

SoftwareSerial nodeMCU(rxPin, txPin);

// WiFi network info.
char ssid[] = "Galaxy A51 917E";
char wifiPassword[] = "tosin@345"; 

// Zafron authentication info.
char username[] = "86c83916-675f-40e6-8b9a-89901f07d8bc";
char password[] = "000000007E162FE3";
char clientID[] = "F11B1035";

int Button;
char c;
String dataIn;
int8_t indexOfA, indexOfB,indexOfC,indexOfD, indexOfE, indexOfF;
String data1, data2, data3, data4, data5, data6;
float ch1, ch2, ch5, ch6;
int ch3, ch4;

void setup() {
  randomSeed(analogRead(0));
    Serial.begin(9600);
    Cayenne.begin(username, password, clientID, ssid, wifiPassword);
    nodeMCU.begin(115200);
}

void loop() {
    Cayenne.loop();
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT(){
    long RandomNumber;
    RandomNumber = random(5);
    RandomNumber *= 10;
    RandomNumber /= 1.21;
    Serial.print("PV Voltage: ");
    Serial.println(RandomNumber);
    recvData();
    // Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
    // Cayenne.virtualWrite(0, millis());
    // Some examples of other functions you can use to send data.
    Cayenne.virtualWrite(1, ch1, "Weather Humidity", "%");
    Cayenne.celsiusWrite(2, ch2);
    Cayenne.luxWrite(3, ch3);
    Cayenne.virtualWrite(4, ch4, "Rain Sensor", "mmHg");
    Cayenne.virtualWrite(5, ch5, "PV Voltage", "V");
    Cayenne.virtualWrite(6, ch6, "Battery Voltage", "V");
    Cayenne.virtualWrite(7, RandomNumber, "PV Current", "A");
    delay(5000);
}

void recvData(){
  while(nodeMCU.available() >0){
  c = nodeMCU.read();
 
   if( c == '\n'){
    break;
  }

  else{
    dataIn += c;
  }
}

if(c == '\n'){
  //Serial.println(c);
  parse_data();

  Serial.println("data 1= " + data1);
  Serial.println("data 2= " + data2);
  Serial.println("data 3= " + data3);
  Serial.println("data 4= " + data4);
  Serial.println("data 5= " + data5);
  Serial.println("data 6= " + data6);
  Serial.println("............................");  
  c = 0;
  dataIn = "";
  }
}


void parse_data(){
  indexOfA = dataIn.indexOf("A");
  indexOfB = dataIn.indexOf("B");
  indexOfC = dataIn.indexOf("C");
  indexOfD = dataIn.indexOf("D");
  indexOfE = dataIn.indexOf("E");
  indexOfF = dataIn.indexOf("F");
 

  data1 = dataIn.substring(0, indexOfA);
  data2 = dataIn.substring(indexOfA+1, indexOfB);
  data3 = dataIn.substring(indexOfB+1, indexOfC);
  data4 = dataIn.substring(indexOfC+1, indexOfD);
  data5 = dataIn.substring(indexOfD+1, indexOfE);
  data6 = dataIn.substring(indexOfE+1, indexOfF);
  
  ch1 = data1.toFloat();
  ch2 = data2.toFloat();
  ch3 = data3.toInt();
  ch4 = data4.toInt();
  ch5 = data5.toFloat();
  ch6 = data6.toFloat();
  
 
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT(){
    CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
    //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}