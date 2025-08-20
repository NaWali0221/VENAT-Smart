#include <Wire.h>
#include "DHT.h"
#include <SPI.h>
#include <WiFi.h>
#include <ThingerESP32.h>
#include <time.h>

//--------------------------------------------------------------------------------------------------

// Define constants
#define THINGER_SERIAL_DEBUG
#define USERNAME "YOUR_USERNAME"
#define DEVICE_ID "YOUR_DEVICE_ID"
#define DEVICE_CREDENTIAL "YOUR_DEVICE_CREDENTIAL"
#define SSID "YOUR_WIFI_SSID"
#define SSID_PASSWORD "YOUR_WIFI_PASSWORD"

//--------------------------------------------------------------------------------------------------

#define VS 18
#define CS 19
#define HS 22
#define HS_SWITCH 21

//--------------------------------------------------------------------------------------------------

#define DHTPIN 2
#define DHTTYPE DHT11
#define LIGHT_SENSOR_PIN 35
#define LIGHT_SYSTEM_PIN 23
#define SOIL_MOISTURE_SENSOR_PIN 34
#define PUMP_PIN 5
#define SECS_PER_DAY 86400

//--------------------------------------------------------------------------------------------------

// Initialize DHT sensor and Thinger.io instance
DHT dht11(DHTPIN, DHTTYPE); 
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

//--------------------------------------------------------------------------------------------------

// Sensor and system variables
float t, h, n, p, k;
int light, soilMoisture;
float energy_usage = 0, energy_savings = 0, cost_savings = 0;
const float pump_power = 2000; // watts
const float light_power = 50;
const float heating_power = 1500;
const float cooling_power = 2000;
const float ventilation_power = 2000;
const float electricity_cost_per_kwh = 0.155850; // local currency per kWh

// Timing variables
unsigned long previousMillis = 0;
const long interval = 2000;
unsigned long lightStartMillis = 0, pumpStartMillis = 0;
unsigned long coolingStartMillis = 0, heatingStartMillis = 0, ventilationStartMillis = 0;
bool lightOn = false, pumpOn = false, coolingOn = false, heatingOn = false, ventilationOn = false;

// Nutrient tracking
float initialN = 35.0, initialP = 5.0, initialK = 50.0;
int daysSincePlanting = 0;

// Growth dates
String germination_date, flowering_date, pollination_date, maturity_date;

//--------------------------------------------------------------------------------------------------

// Add days to a base date
String addDaysToDate(int daysToAdd) {
  struct tm plantingTime = {0,0,0,11,4-1,2025-1900};
  time_t plantingDate = mktime(&plantingTime) + daysToAdd * SECS_PER_DAY;
  struct tm *timeInfo = localtime(&plantingDate);
  char buffer[11];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeInfo);
  return String(buffer);
}

//--------------------------------------------------------------------------------------------------

// Plant growth prediction placeholder
String getPlantGrowthPrediction(float temperature, float humidity, int light, int soilMoisture, float nitrogen, float phosphorus, float potassium) {
    // Proprietary logic removed
    return "Prediction logic removed for privacy";
}

// Update growth stage dates
void updateGrowthDates(String growthStage) {
  germination_date = addDaysToDate(0) + " to " + addDaysToDate(5);
  flowering_date = addDaysToDate(10) + " to " + addDaysToDate(20);
  pollination_date = addDaysToDate(15) + " to " + addDaysToDate(25);
  maturity_date = addDaysToDate(30) + " to " + addDaysToDate(50);
}

//--------------------------------------------------------------------------------------------------

// Pest and disease risk placeholder
float calculatePestDiseaseRisk(float temperature, float humidity, int light, int soilMoisture, float nitrogen, float phosphorus, float potassium) {
    // Proprietary logic removed
    return 0.0;
}

String determineRiskLevel(float risk) {
    // Placeholder logic
    return "Risk analysis logic removed";
}

String getRecommendation(String riskLevel) {
    return "Recommendation logic removed";
}

//--------------------------------------------------------------------------------------------------

// Nutrient depletion forecast placeholder
String forecastNutrientDepletion() {
    return "Nutrient depletion forecast logic removed";
}

//--------------------------------------------------------------------------------------------------

// Soil health monitoring placeholder
String monitorSoilHealth() {
    return "Soil health analysis logic removed";
}

//--------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  srand(time(0));

  // Pin modes
  pinMode(VS, OUTPUT);
  pinMode(HS, OUTPUT);
  pinMode(HS_SWITCH, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(LIGHT_SYSTEM_PIN, OUTPUT);
  pinMode(SOIL_MOISTURE_SENSOR_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);

  // WiFi and sensors
  thing.add_wifi(SSID, SSID_PASSWORD);
  dht11.begin();

  // Thinger.io resources
  thing["dht11"] >> [](pson& out){ out["temperature"]=t; out["humidity"]=h; };
  thing["HS_SWITCH"] << digitalPin(HS_SWITCH);
  thing["light_sensor"] >> [](pson& out){ out["light"]=light; };
  thing["soil_moisture_sensor"] >> [](pson& out){ out["soil_moisture"]=soilMoisture; };
  thing["npk"] >> [](pson& out){ out["nitrogen"]=n; out["phosphorus"]=p; out["potassium"]=k; };
  thing["savings"] >> [](pson& out){ out["energy_usage"]=energy_usage; out["energy_savings"]=energy_savings; out["cost_savings"]=cost_savings; };
  thing["dates"] >> [](pson& out){ 
      out["planting_date"]=addDaysToDate(0); 
      out["germination_date"]=germination_date; 
      out["flowering_date"]=flowering_date; 
      out["pollination_date"]=pollination_date; 
      out["maturity_date"]=maturity_date; 
  };
  thing["system_state"] >> [](pson& out){ 
      out["ventilation"]=ventilationOn?"ON":"OFF"; 
      out["cooling"]=coolingOn?"ON":"OFF"; 
      out["heating"]=heatingOn?"ON":"OFF"; 
      out["lighting"]=lightOn?"ON":"OFF"; 
      out["pump"]=pumpOn?"ON":"OFF"; 
  };
  thing["growth_prediction"] >> [](pson& out){ out["prediction"]=getPlantGrowthPrediction(t,h,light,soilMoisture,n,p,k); };
  thing["pest_disease_risk"] >> [](pson& out){ 
      float risk=calculatePestDiseaseRisk(t,h,light,soilMoisture,n,p,k);
      out["risk_level"]=determineRiskLevel(risk);
      out["recommendation"]=getRecommendation(out["risk_level"]);
  };
  thing["nutrient_depletion_forecast"] >> [](pson& out){ out["forecast"]=forecastNutrientDepletion(); };
  thing["soil_health"] >> [](pson& out){ out["health_status"]=monitorSoilHealth(); };
}

//--------------------------------------------------------------------------------------------------

void loop() {
  thing.handle();
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Read sensors
    t = dht11.readTemperature();
    h = dht11.readHumidity();

    n = 29.9 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (35.0 - 29.9)));
    p = 2.3 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (5.0 - 2.3)));
    k = 45.8 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (50.0 - 45.8)));

    // Environmental controls (ventilation, heating, cooling, lighting, irrigation)
    if (h > 75 && !ventilationOn) { digitalWrite(VS,HIGH); ventilationOn=true; ventilationStartMillis=currentMillis; }
    else if (h <= 75 && ventilationOn) { digitalWrite(VS,LOW); ventilationOn=false; energy_usage+=((currentMillis-ventilationStartMillis)/3600000.0)*(ventilation_power/1000); }

    if (t > 25 && !coolingOn) { digitalWrite(CS,LOW); coolingOn=true; coolingStartMillis=currentMillis; }
    else if (t <= 25 && coolingOn) { digitalWrite(CS,HIGH); coolingOn=false; energy_usage+=((currentMillis-coolingStartMillis)/3600000.0)*(cooling_power/1000); }

    if (t < 20 && !heatingOn) { digitalWrite(HS,LOW); heatingOn=true; heatingStartMillis=currentMillis; }
    else if (t >= 20 && heatingOn) { digitalWrite(HS,HIGH); heatingOn=false; energy_usage+=((currentMillis-heatingStartMillis)/3600000.0)*(heating_power/1000); }

    light = map(analogRead(LIGHT_SENSOR_PIN), 0, 4095, 0, 100);
    if (light < 20 && !lightOn) { digitalWrite(LIGHT_SYSTEM_PIN,HIGH); lightOn=true; lightStartMillis=currentMillis; }
    else if (light >= 20 && lightOn) { digitalWrite(LIGHT_SYSTEM_PIN,LOW); lightOn=false; energy_usage+=((currentMillis-lightStartMillis)/3600000.0)*(light_power/1000); }

    int soilReading = analogRead(SOIL_MOISTURE_SENSOR_PIN);
    soilMoisture = (soilReading > 4096) ? 0 : map(soilReading,0,4096,100,0);

    if (soilMoisture < 30 && !pumpOn) { digitalWrite(PUMP_PIN,HIGH); pumpOn=true; pumpStartMillis=currentMillis; }
    else if (soilMoisture >= 30 && pumpOn) { digitalWrite(PUMP_PIN,LOW); pumpOn=false; energy_usage+=((currentMillis-pumpStartMillis)/3600000.0)*(pump_power/1000); }

    // Energy and cost calculations
    float max_energy_usage=(currentMillis/3600000.0)*(pump_power+light_power+heating_power+cooling_power+ventilation_power)/1000;
    energy_savings=40*(max_energy_usage-energy_usage);
    cost_savings=energy_savings*electricity_cost_per_kwh;

    // Update growth stage and dates
    String growthStage = getPlantGrowthPrediction(t,h,light,soilMoisture,n,p,k);
    updateGrowthDates(growthStage);
  }
}
