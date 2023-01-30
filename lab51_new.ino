// UTFT_Demo_320x240 + tOUCH 7843 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
// for Pollin Eval Board changes by mde Fra-UAS
//
// This program is a demo of how to use most of the functions
// of the library with a supported display modules.
//
// This demo was made for modules with a screen resolution 
// of 320x240 pixels.
//
// This program requires the UTFT library.
//


#include <UTFT.h>
#include <Touch.h>
#include <MapleFreeRTOS900.h>

SemaphoreHandle_t xSemaphore = NULL;



// Declare which fonts we will be using
extern uint8_t SmallFont[];

// Remember to change the model parameter to suit your display module!
UTFT myGLCD(ILI9325D_16,PD11,PD5,PD7,PC1);
Touch ttouch(PA6, PB6, PA5, PB7, PA7);


void car_red_off() {
  myGLCD.setColor(70, 22, 22);
  myGLCD.fillCircle(60, 60, 30);
}

void car_red_on() {
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillCircle(60, 60, 30);
}

void car_yellow_off() {
  myGLCD.setColor(133, 133, 102);
  myGLCD.fillCircle(60, 120, 30);
}

void car_yellow_on () {
  myGLCD.setColor(255, 255, 0);
  myGLCD.fillCircle(60, 120, 30);
}

void car_green_off() {
  myGLCD.setColor(24, 94, 0);
  myGLCD.fillCircle(60, 180, 30);
}

void car_green_on() {
  myGLCD.setColor(31, 207, 0);
  myGLCD.fillCircle(60, 180, 30);
}
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void human_red_off() {
  myGLCD.setColor(70, 22, 22);
  myGLCD.fillCircle(150, 100, 30);
}

void human_red_on() {
  myGLCD.setColor(70, 22, 22);
  myGLCD.fillCircle(150, 100, 30);
}

void human_green_off() {
  myGLCD.setColor(24, 94, 0);
  myGLCD.fillCircle(150, 180, 30);
}

void human_green_on() {
  myGLCD.setColor(31, 207, 0);
  myGLCD.fillCircle(150, 180, 30);
}

void car_traffic_off() {
  vTaskDelay(1000);

  car_green_off();
  car_yellow_on();
  vTaskDelay(1000);
  car_yellow_off();
  car_red_on();
}

void car_traffic_on() {
  vTaskDelay(1000);

  car_yellow_on();
  vTaskDelay(500);
  car_red_off();
  car_yellow_off();
  car_green_on();
}

void human_traffic_off() {
  vTaskDelay(1000);
  human_red_on();
  human_green_off();
  vTaskDelay(1000);
}

void human_traffic_on() {
  vTaskDelay(1000);
  human_red_off();
  human_green_on();
  vTaskDelay(1000);
}

void vTaskCarTrafficLight(void *pvParameters) {

  car_red_off();
  car_yellow_off();
  car_green_off();

  bool light_on = true;
  point_t p;
  
  for (;;) {
    if (xSemaphore != NULL) {
      if (xSemaphoreTake(xSemaphore, (TickType_t) 0) == pdPASS) {
        if (light_on) {
          car_traffic_off();
          light_on = false;
        } else {
          car_traffic_on();
          light_on = true;
        }
        if (ttouch.touchEvent()) {
          p = ttouch.readTouch();
          xSemaphoreGive(xSemaphore);
          Serial.print("X: ");
          Serial.print(p.x);
          Serial.print("Y:  ");
          Serial.println(p.y);
        }
      }
    }
    vTaskDelay(3000);
  }
}

void vTaskHumanTrafficLight(void *pvParameters) {

  human_red_off();
  human_green_off();

  bool light_on = true;
  point_t p;
  
  for (;;) {
    if (xSemaphore != NULL) {
      if (xSemaphoreTake(xSemaphore, (TickType_t) 0) == pdPASS) {
        if (light_on) {
          human_traffic_off();
          light_on = false;
        } else {
          human_traffic_on();
          light_on = true;
        }
        if (ttouch.touchEvent()) {
          p = ttouch.readTouch();
          xSemaphoreGive(xSemaphore);
          Serial.print("X: ");
          Serial.print(p.x);
          Serial.print("Y:  ");
          Serial.println(p.y);
        }
      }
    }
    vTaskDelay(3000);
  }
}


void setup()
{

 
  
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);

  myGLCD.fillRect(200, 40, 300, 80);
  myGLCD.setColor(255, 255, 255);
  ttouch.init(300, 240, 3900, 3900);

  xSemaphore = xSemaphoreCreateBinary();
  xTaskCreate(vTaskCarTrafficLight, "car traffic", 100, NULL, 1, NULL);
  xTaskCreate(vTaskHumanTrafficLight, "car traffic", 100, NULL, 1, NULL);
  xSemaphoreGive(xSemaphore);
  vTaskStartScheduler();
}

void loop()
{


}
