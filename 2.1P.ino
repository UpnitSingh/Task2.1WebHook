
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>


#define DHTPIN 2     
#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);



char ssid[] = "realme 9 5G Speed Edition";   
char pass[] = "12345678";    
WiFiClient  client;

unsigned long myChannelNumber = 2635596;
const char * myWriteAPIKey = "DUILTZ64LKC77PH8";

void setup() {
  Serial.begin(9600);  
  dht.begin();  
  
  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  
  ThingSpeak.begin(client);  
}

void loop() {
  // Read temperature and humidity from the DHT sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the data to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C");
  Serial.print("\tHumidity: ");
  Serial.print(h);
  Serial.println(" %");

  // Write data to ThingSpeak
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
  if(x == 200){
    Serial.println("Data sent to ThingSpeak");
  }
  else{
    Serial.println("Problem sending data. HTTP error code " + String(x));
  }

  // Wait 60 seconds before sending data again
  delay(60000);
}
