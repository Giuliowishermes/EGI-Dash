#include <WiFi.h>
#include <SPI.h>
#include <SD.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define echoPin 5 // Echo Pin
#define trigPin 6 // Trigger Pin
#define maximumRange 200 // Maximum range needed
#define minimumRange 0 // Minimum range needed
#define sensorMin 500       // minimum sensor value
#define sensorMax 0 
#define cella1Pin A0
#define cella2Pin A1


// Ultra
long duration, distance; // Duration used to calculate distance
short zeroUltra;
int dist();
// End Ultra

//Temp and Humidity
DHT dht(DHTPIN, DHTTYPE);
float t;
float h;
//End T&H

//Wifi
char ssid[] = "NetTip";     //  your network SSID (name) 
char pass[] = "ciao@ciao*";    // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
void printWifiStatus();
void httpRequest(String dato);
long lastConnectionTime;
const unsigned long postingInterval = 10L * 1000L; 
char server[] = "innovazioneunito.altrvista.org"; //nostro server 
WiFiClient client;
//End Wifi

short c1, c2;

byte contfile;
byte contapezzi = 0;
File myFile;
String str;
long time = millis();

void setup() {
  
  Serial.begin(115200);
  //TEMP AND HUMIDITY
  Serial.println("DHT22");

  dht.begin();
  Serial.println("DHT22 Inizzializzato");
  delay(200);

  //WIFI
  Serial.println("Attempting to connect to WPA network...");
  status = WiFi.begin(ssid, pass);

  // if you're not connected, stop here:
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection");
    while(true);
  } 
  // if you are connected, print out info about the connection:
  else {
    Serial.println("Connected to network");
    printWifiStatus();
  }
  //END WIFI
  
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  zeroUltra = dist();
  zeroUltra -= 0; 
  Serial.println(zeroUltra);
  
  //SD
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to File...");
    myFile.println("Inizio");
  }
  contfile=0;
  //END SD
  delay(1000);
}

void loop() {
  // Ultra
  c1 = analogRead(cella1Pin);
  c1 = map(c1, sensorMin, sensorMax, -10, 1000);
  if (contapezzi>100 || c1 <= 100){
    Serial.println("Fine carico");
    myFile.print("\n Fine carico, numero pezzi: ");
    myFile.println(contapezzi);
    str.remove(str.length()-1);
    str+= "grammi_rimasti=";
    str+= c1;
    str+= " ";
    httpRequest(str);
    do{
      c1 = analogRead(cella1Pin);
      c1 = map(c1, sensorMin, sensorMax, -10, 1000);
      }while(c1<900);
      contapezzi=0;
      Serial.print("Inizio Nuovo carico con grammi:");
      Serial.println(c1);
      myFile.print("\n Inizio Nuovo carico con grammi:");
      myFile.println(c1);
      str.remove(str.length()-1);
      str+="Carico_grammi=";
      str+= c1;
      str+=" ";
      httpRequest(str);
    }
    
  while(contapezzi<=100 || c1<100){
    distance=dist();
    Serial.println(distance);
    if (distance < zeroUltra){ //controllo se c'è il pezzo
      //qua vedo il peso
      Serial.print("\n E' passato il pezzo, Peso: ");
      c2 = analogRead(cella2Pin);
      c2 = map(c2, sensorMin, sensorMax, -10, 1000);
      Serial.println(c2);
      myFile.print("Pezzo: ");
      myFile.println(c2);
      str.remove(str.length()-1);
      str+="Pezzo=";
      str+= c2;
      str+=" ";
      httpRequest(str);
      contapezzi += 1;
      }
    // Fine Ultra
  
   
    if(millis()>=time+2000){ //ogni 2 secondi stampo temp e umdità
    //Temp e umidità
      t = dht.readTemperature();
      h = dht.readHumidity();
      if (isnan(t) || isnan(h)) {
        Serial.println("Errore nella lettura sens. DHT22");
      } else {
        Serial.println("Umidita: ");
        Serial.println(h);
        Serial.print(" %\t");
        Serial.print("Temperatura: ");
        Serial.print(t);
        Serial.println(" *C");
        str.remove(str.length()-1);
        str+="Temp=";
        str+= t;
        str+=" ";
        httpRequest(str);
        str.remove(str.length()-1);
        str+="Umidita=";
        str+= h;
        str+=" ";
        httpRequest(str);
      }
    }
    //Fine temp e umidità
    contfile += 1;
    if (contfile>=70){
      myFile.flush(); contfile=0;}
      
    delay(1000);
  }
}

int dist(){
  bool flag=false;
  do{
    if (flag==true){Serial.println("\n Error Ulta sens."); flag=false;}
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
 
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  Serial.println(distance);
  delay(500);
  flag=true;
  }while (distance >= 200 || distance <= 0);
  
  return distance;
 }

 void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void scritturaSd(){
  if (myFile) {
    Serial.println("Writing to File...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  
}

void httpRequest(String dato) {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  if (client.connect(server, 80)) {
    Serial.println("connecting...");

    Serial.println("debug 1");
    client.print("GET /index.php?");
    client.print(dato);
    client.println(" HTTP/1.1");
    client.println("Host: innovazioneunito.altervista.org");
    client.println("Connection: close");
    client.println();

    delay(1000);

  } 
  else {
    // If you couldn't make a connection:
    Serial.println("Connection failed");
    Serial.println("Disconnecting.");
    client.stop();
  }
  Serial.println("fine richiesta");
}


