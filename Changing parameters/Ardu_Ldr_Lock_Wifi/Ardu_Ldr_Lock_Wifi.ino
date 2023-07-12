#include <ESP8266HTTPClient.h>

#include <Arduino.h>
#include <ESP8266WiFi.h>

// Pins
#define LIGHT_SENSOR_PIN A0
#define RELAY_PIN D1
#define PWM_PIN D2

//Setting Variables
int relayDelay = 2000; 
int lightSensorDelay = 2000;
int pwmDelay = 2000; 
int standbyDelay = 2000;

// Light Sensor Variables
long totalLightLevel = 0;
int lightReadings = 0;
int averageLightLevel = 0;

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);
  wifi_setup(); // Defined in the second file
}

void loop() {
  wifi_loop();
  relay_loop();
  lightSensor_loop();
  pwm_loop();
  standby_loop();
  
}

void relay_loop() {
    delay(relayDelay);
    digitalWrite(RELAY_PIN, !digitalRead(RELAY_PIN));
    Serial.println("Relay switched");
    Serial.print("Current Relay Delay: ");
    Serial.println(relayDelay);
  }


void lightSensor_loop() {
  
  while(lightReadings!=lightSensorDelay/1000) {
    int lightLevel = analogRead(LIGHT_SENSOR_PIN);
  delay(1000);
  totalLightLevel += lightLevel;
  lightReadings++;
  Serial.print("number of read: ");
  Serial.println(lightReadings);
  }
    
    averageLightLevel = totalLightLevel / lightReadings;
    totalLightLevel = 0;
    lightReadings = 0;

    Serial.print("Average light level: ");
    Serial.println(averageLightLevel);
    Serial.print("Current Light Sensor Delay: ");
    Serial.println(lightSensorDelay);
  }


void pwm_loop() {
    delay(pwmDelay);
    int pwmValue = averageLightLevel;
    analogWrite(PWM_PIN, pwmValue);
    Serial.print("PWM set to: ");
    Serial.println(pwmValue);
    Serial.print("Current PWM Delay: ");
    Serial.println(pwmDelay);
  }


void standby_loop() {
    delay(standbyDelay);
    Serial.println("Standby switched");
    Serial.print("Current Standby Delay: ");
    Serial.println(standbyDelay);
  }
