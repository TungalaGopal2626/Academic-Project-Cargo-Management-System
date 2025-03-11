#include <DHT.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define DHTPIN D2        // Pin for DHT11
#define DHTTYPE DHT11    // DHT 11 sensor type
#define MQ7_PIN A0       // MQ-7 sensor analog pin

DHT dht(DHTPIN, DHTTYPE);

#define WIFI_SSID "test"
#define WIFI_PASSWORD "12345678"
#define API_KEY "AIzaSyC4xHYk-ToMWnKKa5l5J1Nxg8Jj_MQIEZQ"
#define DATABASE_URL "https://cloud-81333-default-rtdb.firebaseio.com/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup() {
    Serial.begin(9600);
    dht.begin();

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;
    if (Firebase.signUp(&config, &auth, "", "")) {
        Serial.println("Firebase Auth successful");
        signupOK = true;
    } else {
        Serial.printf("Firebase Auth failed: %s\n", config.signer.signupError.message.c_str());
    }

    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

void loop() {
    // Read MQ-7 gas sensor data
    int gasValue = analogRead(MQ7_PIN);
    float voltage = gasValue * (5.0 / 1023.0); // Convert ADC value to voltage
    
    // Read DHT11 temperature and humidity
    float temperature = dht.readTemperature(); // Read temperature in Celsius
    float humidity = dht.readHumidity();       // Read humidity

    // Check if readings are valid
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Print data to Serial Monitor
    Serial.print("CO Sensor Value: ");
    Serial.print(gasValue);
    Serial.print(" | Voltage: ");
    Serial.print(voltage);
    Serial.println("V");

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C | Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    // Alert for High Temperature
    if (temperature > 35.0) {
        Serial.println("⚠️ ALERT: Decrease Temperature!");
    }

    // Alert for Food Spoilage
    if (gasValue > 550) {
        Serial.println("⚠️ ALERT: Food Spoiled!");
    }

    Serial.println("----------------------");
    delay(2000); // Wait 2 seconds before next reading

    // Send data to Firebase
    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
        sendDataPrevMillis = millis();

        if (Firebase.RTDB.setInt(&fbdo, "mainbucket/temperature", temperature)) {
            Serial.println("Temperature sent to Firebase.");
        } else {
            Serial.println("Failed to send temperature data: " + fbdo.errorReason());
        }

        if (Firebase.RTDB.setInt(&fbdo, "mainbucket/gasValue", gasValue)) {
            Serial.println("Gas value sent to Firebase.");
        } else {
            Serial.println("Failed to send gas value data: " + fbdo.errorReason());
        }

        if (Firebase.RTDB.setInt(&fbdo, "mainbucket/humidity", humidity)) {
            Serial.println("Humidity sent to Firebase.");
        } else {
            Serial.println("Failed to send humidity data: " + fbdo.errorReason());
        }
    }
}
