#include <M5Stack.h>
#include "ThingSpeak.h"
#include <Wire.h>
#include <WiFi.h>
#include "SHT3X.h"

// relay PIN
#define RELAY_PIN 26

// WiFi settings 
const char* ssid = ""; // ENTER SSID
const char* password = ""; // ENTER PASSWORD

//thingSpeak settings 
unsigned long myChannelNumber = 2329291;
const char* myWriteApiKey = "C2O971A1LP0NXW2Z";
const char* myCounterReadAPIKey = "KFZZNTR3Z957ALL6";

// object for network configuration
WiFiClient client;

// termperature and humidity sensor
SHT3X sht30;

// variables init (default settings)
float temperatura = 0.0;
float vlaga = 0.0;
float maxTemp = 25.0; // default maximum temperature
float maxVlaga = 50.0; // default maximum humidity
float manual = 0.0; // default manual open=1/close=0


void setup() {
  // Inicializacija
  M5.begin(); //Init M5Stack. 
  M5.Power.begin(); //Init power
  
  ///////////////////////////////////////////////////////////////////////////////////////////////
  // display settings (brightness, text size, color)
  M5.Lcd.setBrightness(10);
  M5.Lcd.setTextSize(2);
  Serial.println(F("Temperatura in vlaga"));
  M5.Lcd.clear(BLACK);
  M5.Lcd.println("Temperatura in vlaga");

  ///////////////////////////////////////////////////////////////////////////////////////////////
  // connecting to wifi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.println("Connecting...");
  }
  Serial.println("Connected!");
  pinMode(RELAY_PIN, OUTPUT); // PIN initialization OUTPUT
  digitalWrite(RELAY_PIN, HIGH); // PIN initialization HIGH voltage (OFF)
  
  ThingSpeak.begin(client); // connect to Thingspeak service
}

void loop(void) {

  // reading manual opening value from thingspeak
  manual = ThingSpeak.readFloatField(myChannelNumber, 5, myCounterReadAPIKey);
  Serial.println(manual);
  // reading maximum values from thingspeak
  maxTemp =  ThingSpeak.readFloatField(myChannelNumber, 3, myCounterReadAPIKey);
  Serial.println(maxTemp);
  maxVlaga =  ThingSpeak.readFloatField(myChannelNumber, 4, myCounterReadAPIKey);
  Serial.println(maxVlaga);
  
/////////////////////////////////////////////////////////////////////////////////////////////////
  // condition for manual opening
  if(manual==1.0) {
    digitalWrite(RELAY_PIN, LOW);
    M5.Lcd.setCursor(0, 120);
    Serial.println("Rocno odpiranje. Odpiram okno !");
    M5.Lcd.println("Rocno odpiranje. Odpiram okno !");
  } else {

    // conditions for automatic window opening
    if ((temperatura > maxTemp) || (vlaga > maxVlaga)) {
      digitalWrite(RELAY_PIN, LOW);  // ON - low woltage
      M5.Lcd.setCursor(0, 120);
      Serial.println("Presezene vrednosti. Odpiram okno !");
      M5.Lcd.println("Presezene vrednosti. Odpiram okno !");
    } else {
      digitalWrite(RELAY_PIN, HIGH);  // OFF - high voltage
      M5.Lcd.setCursor(0, 120);
      M5.Lcd.println("Vrednosti v mejah normale. Okno je zaprto !");
      Serial.println("Vrednosti v mejah normale. Okno je zaprto !");
    }
  }


  // sensor reading
  if (sht30.get() == 0) {
  temperatura = sht30.cTemp;
  vlaga = sht30.humidity;
  Serial.printf("Sensor Reading - Temperature: %2.2f°C  Humidity: %2.2f%%\n", temperatura, vlaga);
  } else {
  Serial.println("Error reading sensor.");
  }
  
  
  ///////////////////////////////////////////////////////////////////////////////////////////////
  // display update
  Serial.printf("Temperatura: %2.2f*C  Vlaga: %0.2f%%  \r\n", temperatura, vlaga);
  M5.Lcd.setCursor(0, 40);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.printf("Temperatura: %2.1f  \r\nVlaga: %2.0f%%  \r\n", temperatura, vlaga);
  M5.Lcd.setCursor(0,90);
  delay(100);

  ///////////////////////////////////////////////////////////////////////////////////////////////
  /// setting data fields to be sent
  ThingSpeak.setField(1, temperatura);
  ThingSpeak.setField(2, vlaga);
  //Thingspeak.setField(5, 0); // setting field manual open back to close  
  // sending data fields, 'statusCode' is server response
  int statusCode = ThingSpeak.writeFields(myChannelNumber, myWriteApiKey);

  // succesful or unsuccessful update
  if(statusCode == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(statusCode));
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////
  // 20 delay for next reading/writing
  delay(20000); 
}
