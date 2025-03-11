Two codes
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include "DHTesp.h"

#define WIFI_SSID "test"
#define WIFI_PASSWORD "12345678"
#define API_KEY "AIzaSyCfagYS4c8SvrSwuGT6XSWQP1ABSAPfSRM"
#define DATABASE_URL "https://nodemcu-abee1-default-rtdb.firebaseio.com/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
DHTesp dht;
int lpin = 16;
String intValue;

void setup(){
  pinMode(lpin, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);
  dht.setup(16, DHTesp::DHT11);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Firebase Auth successful");
    signupOK = true;
  }
  else{
    Serial.printf("Firebase Auth failed: %s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  // DHT Sensor Reading
  delay(dht.getMinimumSamplingPeriod());
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  
  // Firebase Data Sending
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    if (Firebase.RTDB.setInt(&fbdo, "mainbucket/temp", temperature)){
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("Failed to send temperature data. Reason: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setInt(&fbdo, "mainbucket/humid", humidity)){
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("Failed to send humidity data. Reason: " + fbdo.errorReason());
    }
  }

  // LED Control
  if (Firebase.ready() && signupOK)
  {
    if (Firebase.RTDB.getString(&fbdo, "/mainbucket/bulb"))
    {
      intValue = fbdo.stringData();
      String mySubString = intValue.substring(2, 3);
      Serial.println(intValue);
      Serial.println(mySubString);
      if (mySubString == "0")
      {
        digitalWrite(lpin, LOW);
        Serial.println("LED OFF");
        delay(100);
      }
      else if (mySubString == "1")
      {
        digitalWrite(lpin, HIGH);
        Serial.println("LED ON");
        delay(100);
      }  
      delay(100);
    }
    else {
      Serial.println("Firebase error: " + fbdo.errorReason());
    }
    delay(100);
  }
}
