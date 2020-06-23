#include <WiFiNINA.h>
#include <SPI.h>
#include <ArduinoBLE.h>
#include "arduino_secrets.h"
#define LOG_PERIOD 5000  //Logging period in milliseconds, recommended value 15000-60000.
#define MAX_PERIOD 60000  //Maximum logging period without modifying this sketch

// Wifi Ayarları
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                           // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;




// Geiger Ayarları
unsigned long counts;     //variable for GM Tube events
unsigned long cpm;        //variable for CPM
float dose;
unsigned int multiplier;  //variable for calculation CPM in this sketch
unsigned long previousMillis;  //variable for time measurement
void tube_impulse(){       //subprocedure for capturing events from Geiger Kit
  counts++;
}


char server[] = "192.168.1.128";
WiFiClient client;









void setup(){             //setup subprocedure
  counts = 0;
  cpm = 0;
  multiplier = MAX_PERIOD / LOG_PERIOD;      //calculating multiplier, depend on your log period
  Serial.begin(9600);
  attachInterrupt(0, tube_impulse, FALLING); //define external interrupts 




// Wifi Kontrol
  if (WiFi.status() == WL_NO_SHIELD) {
    //Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // Wifi Ağına Bağlan
  while ( status != WL_CONNECTED) {
    Serial.print("Wifi Ağına Bağlanıyor.");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // Bağlantı için 10 Saniye Bekle
    delay(10000);
  }

  // Bağlanıldıysa Yazdır
 Serial.print("Wifi Ağına Bağlanıldı.");
  printWiFiStatus();




  


  
}




void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  //Serial.print("signal strength (RSSI):");
  //Serial.print(rssi);
  //Serial.println(" dBm");
}

void print2digits(int number) {
  if (number < 10) {
    //Serial.print("0");
  }
  //Serial.print(number);
}








void loop(){                                 //main cycle
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > LOG_PERIOD){
    previousMillis = currentMillis;
    cpm = counts * multiplier;
    //Serial.println(cpm);
    dose = cpm / 151.0;
    Sending_To_phpmyadmindatabase(); 
    Serial.println(dose);
    //Serial.print(cpm);
    counts = 0;
  }
}


  void Sending_To_phpmyadmindatabase()   //CONNECTING WITH MYSQL
 {
   if (client.connect(server, 80)) {
    //Serial.println("connected");
    // Make a HTTP request:
    //Serial.print("GET /testcode/dht.php?cpm=");
    client.print("GET /testcode/dht.php?cpm=");     //YOUR URL
    //Serial.println(cpm);
    client.print(cpm);
    client.print("&dose=");
    //Serial.println("&dose=");
    client.print(dose);
    //Serial.println(dose);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: 144.122.31.55");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    //Serial.println("connection failed");
  }
 }
 
 
