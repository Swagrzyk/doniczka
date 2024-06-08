#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#define pump_PIN   16  // The ESP32 pin GPIO13 that connects to the relay to control the pump
#define waterSensorPower_PIN   32  // The ESP32 pin GPIO32 that provides the power to the water sensor
#define waterSensor_PIN  34  // The ESP32 pin GPIO36 that reads the value from the water sensor
#define soilMoisture_PIN 36 // ESP32 pin GPIO36 (ADC0) that connects to AOUT pin of moisture sensor

// #define waterThreshold  1000 // The threshold for water detectiion
#define moistureThreshold  60 // The threshold for soil moisture detection (60%)

const char* ssid = "ENTER_WIFI_SSID"; 
const char* password = "ENTER_WIFI_PASSWORD"; 

String postData = ""; //--> Variables sent for HTTP POST request data.
String payload = "";  //--> Variable for receiving response from HTTP POST.
int httpCode = 0; //--> Variables for HTTP return code.

// int waterLevel = 0;
int soilMoistureLevel = 0;
int temperature = 23; // 23 stopnie celsjusza

// void connectWiFi() {
//   WiFi.mode(WIFI_OFF);
//   delay(1000);
//   WiFi.mode(WIFI_STA);
  
//   WiFi.begin(ssid, password);
//   Serial.println("Connecting to WiFi");
  
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
    
//   Serial.print("connected to : "); 
//   Serial.println(ssid);
//   Serial.print("IP address: "); 
//   Serial.println(WiFi.localIP());
// }

void setup() {

//    if(WiFi.status() != WL_CONNECTED) {
//       connectWiFi();
//    }

    Serial.begin(115200); // Inicjalizacja komunikacji szeregowej

    pinMode(pump_PIN, OUTPUT); // ustawia pin pompy(13) jako wyjscie
    pinMode(waterSensorPower_PIN, OUTPUT); // ustawia pin czujnik poziomu cieczy jako wyjscie
    digitalWrite(pump_PIN, LOW); // wyłącza pompę
    digitalWrite(waterSensorPower_PIN, LOW); // wyłącza czujnik poziomu cieczy
}

void loop() {
    soilMoistureLevel = analogRead(soilMoisture_PIN); // Odczytuje stan czujnika wilgotnosci gleby, pokazuje się 1 albo 0 (stan wysoki/niski)
    _soilMoistureLevel = ( 100 - ( (soilMoistureLevel/4095.00) * 100 ) );
    Serial.print(_soilMoistureLevel);           /* Print Temperature on the serial window */
    Serial.println("%");

    digitalWrite(waterSensorPower_PIN, HIGH);  // turn the water sensor's power  ON
    delay(10);                      // wait 10 milliseconds
    waterLevel = digitalRead(waterSensor_PIN); // Odczytaj stan czujnika wody (cyfrowo) 
    digitalWrite(waterSensorPower_PIN, LOW);   // turn the water sensor's power OFF
    Serial.print(waterLevel); 

    HTTPClient http; //--> Declare object of class HTTPClient.

    postData = "id=esp32_01";
    postData = "&temperature=" + String(temperature) 
    postData += "&humidity=" + String(_soilMoistureLevel) 
    //postData += "&waterLevel=" + String(waterLevel);

    // In this project I use local server or localhost with XAMPP application.
    // So make sure all PHP files are "placed" or "saved" or "run" in the "htdocs" folder.
    // I suggest that you create a new folder for this project in the "htdocs" folder.
    // The "htdocs" folder is in the "xampp" installation folder.
    // The order of the folders I recommend:
    // xampp\htdocs\your_project_folder_name\phpfile.php
    //
    // ESP32 accesses the data bases at this line of code: 
    // http.begin("http://REPLACE_WITH_YOUR_COMPUTER_IP_ADDRESS/REPLACE_WITH_PROJECT_FOLDER_NAME_IN_htdocs_FOLDER/getdata.php");
    // REPLACE_WITH_YOUR_COMPUTER_IP_ADDRESS = there are many ways to see the IP address, you can google it. 
    //                                         But make sure that the IP address used is "IPv4 address".
    // Example : http.begin("http://192.168.0.1/ESP32_MySQL_Database/Final/getdata.php");
    http.begin("http://REPLACE_WITH_YOUR_COMPUTER_IP_ADDRESS/REPLACE_WITH_PROJECT_FOLDER_NAME_IN_htdocs_FOLDER/updateDHT11data_and_recordtable.php");  //--> Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //--> Specify content-type header
    
    httpCode = http.POST(postData);
    payload = http.getString();
    Serial.print("httpCode : ");
    Serial.println(httpCode); //--> Print HTTP return code
    Serial.print("payload  : ");
    Serial.println(payload);  //--> Print request response payload

    if (waterLevel == HIGH) {
    Serial.println("Woda wykryta"); // Jeśli czujnik wykryje wodę, wyświetl komunikat
        if(_soilMoistureLevel < moistureThreshold) {
            digitalWrite(pump_PIN, 50); // prędkość z jaką ma się kręcić silnik (0-255), ewentualnie dać tu na HIGH - podlewanie robi brrrr
            delay(5000); // poczekaj 5s
            digitalWrite(pump_PIN, LOW); // wyłącza pompę
        }
        else {
            digitalWrite(pump_PIN, LOW); // wyłącza pompę
        }
    } else {
    Serial.println("Brak wody"); // Jeśli czujnik nie wykryje wody, wyświetl komunikat
    }
    delay(3600000); // Poczekaj godzinę przed kolejną akcją
}
