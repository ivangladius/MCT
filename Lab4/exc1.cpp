#include <MapleFreeRTOS900.h>
#include <Touch.h>
#include <UTFT.h>

UTFT myGLCD(ILI9325D_16, PD11, PD5, PD7, PC1);
Touch ttouch(PA6, PB6, PA5, PB7, PA7);

extern uint8_t SmallFont[];

int r = 0;
int g = 0;
int b = 0;

bool zeichnen = false;

int X = 0;
int Y = 25;

void colors() {
  myGLCD.fillRect(0, 0, 40, 20);
  myGLCD.setColor(0, 0, 0);

  myGLCD.fillRect(40, 0, 80, 20);
  myGLCD.setColor(0, 0, 255);

  myGLCD.fillRect(80, 0, 120, 20);
  myGLCD.setColor(0, 255, 255);

  myGLCD.fillRect(120, 0, 160, 20);
  myGLCD.setColor(255, 0, 0);

  myGLCD.fillRect(160, 0, 200, 20);
  myGLCD.setColor(255, 0, 255);

  myGLCD.fillRect(200, 0, 240, 20);
  myGLCD.setColor(255, 255, 0);

  myGLCD.fillRect(240, 0, 280, 20);
  myGLCD.setColor(0, 255, 0);

  myGLCD.fillRect(280, 0, 319, 20);
  myGLCD.setColor(255, 255, 255);
}

void buttons() {
  myGLCD.drawRect(0, 220, 100, 239);
  myGLCD.print("Start", 25, 225);

  myGLCD.drawRect(100, 220, 220, 239);
  myGLCD.print("Clear", 130, 225);

  myGLCD.drawRect(220, 220, 319, 239);
  myGLCD.print("Stop", 240, 225);
}
void drawingarea() {
  myGLCD.fillRect(0, 20, 319, 220);
  myGLCD.setColor(255, 255, 255);
}

void task1(void *pvParamters) {

  while (true) {

    if (zeichnen == 1) {

      if (X < 320) {
        myGLCD.drawPixel(X, Y);
        myGLCD.setColor(r, g, b);
        X++;
        if (X == 319) {
          X = 0;

          if (Y < 220) {
            Y++;
          }
          if (Y == 220) {
            Y = 25;
          }
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void task2(void *pvParameters) {

  point_t p;

  while (true) {

    while (ttouch.touchEvent() == 0) {
      vTaskDelay(pdMS_TO_TICKS(10));
    }

    p = ttouch.readTouch();

    if ((p.y >= 0) && (p.y < 20)) // Farbwahl Button

      if ((p.x >= 0) && (p.x < 40)) {
        r = 255;
        g = 255;
        b = 255;
      }
    if ((p.x >= 40) && (p.x < 80)) {
      r = 0;
      g = 0;
      b = 0;
    }
    if ((p.x >= 80) && (p.x < 120)) {
      r = 0;
      g = 0;
      b = 255;
    }
    if ((p.x >= 120) && (p.x < 160)) {
      r = 0;
      g = 255;
      b = 255;
    }
    if ((p.x >= 160) && (p.x < 200)) {
      r = 255;
      g = 0;
      b = 0;
    }
    if ((p.x >= 200) && (p.x < 240)) {
      r = 255;
      g = 0;
      b = 255;
    }
    if ((p.x >= 240) && (p.x < 280)) {
      r = 255;
      g = 255;
      b = 0;
    }
    if ((p.x >= 280) && (p.x < 319)) {
      r = 0;
      g = 255;
      b = 0;
    }
    if ((p.y >= 220) && (p.y < 239)) { // START,STOP,CLEAR Button

      if ((p.x >= 0) && (p.x < 100)) { // Start
        zeichnen = true;
      }
      if ((p.x >= 100) && (p.x < 220)) { // Clear

        zeichnen = false;
        myGLCD.fillRect(0, 20, 319, 220);
        myGLCD.setColor(255, 255, 255);
      }
      if ((p.x >= 220) && (p.x < 319)) { // Stop
        zeichnen = false;
      }
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);

  ttouch.init(320, 240, 3900, 3900);

  typedef struct Point {
    uint16_t x;
    uint16_t y;
  } point_t;

  colors();
  buttons();
  drawingarea();

  xTaskCreate(task1, "Pixel", 100, NULL, 2, NULL);
  xTaskCreate(task2, "touchPixel", 100, NULL, 2, NULL);
  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:
}
