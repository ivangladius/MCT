
#include <MapleFreeRTOS900.h>
#include <UTFT.h>
#include <Touch.h>


UTFT myDisplay(ILI9325D_16, PD11, PD5, PD7, PC1);
Touch myTouch(PA6, PB6, PA5, PB7, PA7);


SemaphoreHandle_t xSemaphore = NULL;



// start with car traffic light on green , human on red
bool car_traffic_button = true;
bool human_traffic_button = false;

/* ### IMPORTANT ###
 *
 * _off -> dark colors
 * _on() -> bright colors
 *
 */

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
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

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$

void vTaskReader(void *pvParameters) {

  point_t point;

  for (;;) {

    while(myTouch.touchEvent() == 0)
      vTaskDelay(50);

    point = myTouch.readTouch();

    if (220 <= point.x && point.<= 260) {
      if (80 <= point.y && point.y <= 120) 
        car_traffic_button = true;
      else if (150 <= point.y && point.y <= 190) 
        human_traffic_button = true;
    }
  }
}

void vTaskDraw(void *pvParameters) {

  for (;;) {

    // car traffic on & human off
    if (car_traffic_button) { 
      vTaskDelay(1000);
      car_yellow_on();
      vTaskDelay(500);
      car_red_off();
      car_yellow_off();
      car_green_on();

      human_green_off();
      human_red_on();

      car_traffic_button = false;

    }
    // human traffic on && car off
    else if (human_traffic_button) { 
      vTaskDelay(1000);
      car_green_off();
      car_yellow_on();
      vTaskDelay(500);
      car_yellow_off();
      car_red_on();

      human_green_on();
      human_red_off();

      human_traffic_button = false;
    }
  }
}



void setup() {

  myDisplay.InitLCD();
  myTouch.init(320, 240, 3900, 3900);

  // traffic button
  myDisplay.drawRect(220, 80, 260, 120);
  // human button
  myDisplay.drawRect(220, 150, 260, 190);

  xTaskCreate(vTaskReader, "Reader", 100, NULL, 1, NULL);
  xTaskCreate(vTaskDraw, "Draw", 100, NULL, 1, NULL);

  xSemaphoreGive(xSemaphore);

  vTaskStartScheduler();  
}

void loop() { }

