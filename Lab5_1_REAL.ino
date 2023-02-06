#include <MapleFreeRTOS900.h>
#include <UTFT.h>
#include <Touch.h>

UTFT myGLCD(ILI9325D_16, PD11, PD5, PD7, PC1);
Touch myTouch(PA6, PB6, PA5, PB7, PA7);

point_t touchPoint;

// zustaende: r = 1; y = 2; ry = 3; g = 4

int TrafficLight = 1; // initial to red
int pedLight = 1; // initial to red
int Switch_light = 1;

bool inductiveSensor = false;
bool button = false;

SemaphoreHandle_t xSemaphore = NULL;

void setup() {
  myGLCD.InitLCD();
  myTouch.init(320, 240, 3900, 3900);
  setupScreen();
  xSemaphore = xSemaphoreCreateBinary();
  xTaskCreate(vTrafficlight, "Task1", 100, NULL, 1, NULL);
  xTaskCreate(vPedestrianlight, "Task2", 100, NULL, 1 ,NULL);
  xTaskCreate(vDraw, "Task3", 100, NULL, 1, NULL);
  xTaskCreate(vTouchzone, "Task4", 100, NULL, 1, NULL);
  xSemaphoreGive(xSemaphore);

  vTaskStartScheduler();  
}

void setupScreen(){
 // myGLCD.clrScr();
  myGLCD.fillScr(255,255,255); //white
  myGLCD.setColor(VGA_BLUE);
  myGLCD.fillRect(240, 0, 319, 119);
  myGLCD.setColor(VGA_GREEN);
  myGLCD.fillRect(240, 120, 319, 239);
  myGLCD.setColor(0,0,0); //black
  myGLCD.drawRect(30, 200, 220, 140); // Ampel rect
  myGLCD.drawRect(90, 100, 220, 40); // Ampel rect
  myGLCD.drawRect(240, 1, 318, 120); // sensor rect
  myGLCD.drawRect(240, 121, 318, 238); // sensor rect
}

void resettoInit(){
  myGLCD.setColor(0,0,0); //Black
  myGLCD.fillCircle(60, 170, 25); // rCar
  myGLCD.fillCircle(120, 170, 25); // yowCar
  myGLCD.fillCircle(180, 170, 25); // gCar
  myGLCD.fillCircle(120, 70, 25); // rPed
  myGLCD.fillCircle(180, 70, 25); // gPed
}

void vPedestrianlight(void* pvParameters){
  while(true){
    if(xSemaphore != NULL){
      while(!button){ 
        vTaskDelay(50); 
      }
      if(xSemaphoreTake( xSemaphore, (TickType_t) 0) == pdPASS){
        if(TrafficLight == 4){
          TrafficLight = 2;
          Switch_light = 1;
          vTaskDelay(1000);
          TrafficLight = 1;
          Switch_light = 1;
          //vTaskDelay(1000);
        }
        pedLight = 4;
        Switch_light = 1;
        vTaskDelay(2000);
        button = false;
        xSemaphoreGive(xSemaphore);
        vTaskDelay(1);   
      }
    }
  }
  vTaskDelete(NULL);
}

void vTrafficlight(void* pvParameters){
  while(true){
    while(!inductiveSensor){ 
      vTaskDelay(50); 
    }
    
    if(xSemaphore != NULL){
      if(xSemaphoreTake( xSemaphore, (TickType_t) 0) == pdPASS){
        if(pedLight == 4){
          //vTaskDelay(1000);
          pedLight = 1;
          Switch_light = 1;
          //vTaskDelay(1000);
        }
        TrafficLight = 3;
        Switch_light = 1;
        vTaskDelay(1000);
        TrafficLight = 4;
        Switch_light = 1;
        vTaskDelay(400);
        inductiveSensor = false;
        xSemaphoreGive(xSemaphore);
        vTaskDelay(1);
      }
    }
  }
  vTaskDelete(NULL);
  
}

void vDraw(void* pvParameters){
  while(true){ 
    if(Switch_light == 1){
      resettoInit();
      
      if(TrafficLight == 1){
          myGLCD.setColor(VGA_RED);
          myGLCD.fillCircle(60, 170, 25); // rCar
      } else if(TrafficLight == 2) {
          myGLCD.setColor(VGA_YELLOW);
          myGLCD.fillCircle(120, 170, 25); // yowCar
      } else if(TrafficLight == 3) {
          myGLCD.setColor(VGA_RED);
          myGLCD.fillCircle(60, 170, 25); // rCar
          myGLCD.setColor(VGA_YELLOW);
          myGLCD.fillCircle(120, 170, 25); // yowCar
      } else if(TrafficLight == 4) {
          myGLCD.setColor(VGA_GREEN);
          myGLCD.fillCircle(180, 170, 25); // gCar
      }
 
      if(pedLight == 1){
          myGLCD.setColor(VGA_RED);
          myGLCD.fillCircle(120, 70, 25); // rPed
      } else if(pedLight == 4) {
          myGLCD.setColor(VGA_GREEN);
          myGLCD.fillCircle(180, 70, 25); // gPed
      }
      
      Switch_light = false;      // change the lights
    }
    vTaskDelay(50);
  }
  vTaskDelete(NULL);
}


void vTouchzone(void* pvParameters){
  point_t point;
  while(true){
    while(myTouch.touchEvent() == 0){
      vTaskDelay(10);
    }
    point = myTouch.readTouch();
  //  vTaskDelay(50);
  Serial.print("X: ");
  Serial.print(point.x);
  Serial.print(" Y: ");
  Serial.print(point.y);
    if(point.x >= 240 && point.x <= 320){
      if(point.y < 120){
        Serial.println("PED");
        if(pedLight == 1) button = true;
      }if(point.y <= 230) {
        Serial.println("CAR");
        if(TrafficLight == 1) inductiveSensor = true;
      }
    }
  }
  vTaskDelete(NULL);
}

void loop() {
}
