#include <DHT.h>

#define DHTPIN D2         // Pin for DHT11
#define DHTTYPE DHT11    // DHT 11 sensor type
#define MQ7_PIN A0       // MQ-7 sensor analog pin

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    dht.begin();
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

    delay(2000); // Wait 2 seconds before next reading
}
