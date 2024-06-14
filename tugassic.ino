#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 27
#define DHTTYPE DHT11   

const char* ssid = "Dz";
const char* password = "12345678";
const char* serverAddress = "http://192.168.43.66:5000/sensor/suhu";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Connecting to WiFi...");
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  dht.begin();
}

void loop() {
  delay(2000);
  
  float temperature = dht.readTemperature();
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  sendTemperatureToServer(temperature);
}

void sendTemperatureToServer(float temperature) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverAddress);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    String postBody = "temperature=" + String(temperature);
    int httpResponseCode = http.POST(postBody);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Data sent to server!");
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}



