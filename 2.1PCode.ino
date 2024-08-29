// Import necessary libraries
#include <WiFiNINA.h>  // Library for WiFi connectivity
#include <ThingSpeak.h>  // Library for ThingSpeak API
#include <DHT.h>  // Library for DHT temperature and humidity sensor

// Define constants for DHT sensor
#define DHTPIN 2     // Pin number for DHT sensor
#define DHTTYPE DHT22  // Type of DHT sensor (DHT22)

// Create a DHT object
DHT dht(DHTPIN, DHTTYPE);

// Define WiFi credentials
char ssid[] = "realme 9 5G Speed Edition";  // WiFi network name
char pass[] = "12345678";  // WiFi network password

// Create a WiFi client object
WiFiClient client;

// Define ThingSpeak channel and API key
unsigned long myChannelNumber = 2635596;  // ThingSpeak channel number
const char * myWriteAPIKey = "DUILTZ64LKC77PH8";  // ThingSpeak write API key

// Setup function (runs once at the beginning)
void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(9600);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  // Wait for WiFi connection to establish
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  
  // Initialize ThingSpeak API
  ThingSpeak.begin(client);
}

// Loop function (runs repeatedly)
void loop() {
  // Read temperature and humidity from DHT sensor
  float h = dht.readHumidity();  // Read humidity
  float t = dht.readTemperature();  // Read temperature

  // Check if any reads failed
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print data to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C");
  Serial.print("\tHumidity: ");
  Serial.print(h);
  Serial.println(" %");

  // Write data to ThingSpeak
  ThingSpeak.setField(1, t);  // Set temperature field
  ThingSpeak.setField(2, h);  // Set humidity field

  // Send data to ThingSpeak
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
  // Check if data was sent successfully
  if (x == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Problem sending data. HTTP error code " + String(x));
  }

  // Wait 60 seconds before sending data again
  delay(60000);
}