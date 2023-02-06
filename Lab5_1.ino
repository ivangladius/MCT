
#include <MapleFreeRTOS900.h>


void vTaskCar(void *pvParameters) {
  
}

void vTaskHuman(void *pvParameters) {
  
}

void setup() {
  // put your setup code here, to run once:
  xTaskCreate(vTaskCar, "Task car", 100, NULL, 1, NULL);
  xTaskCreate(vTaskHuman, "Task human", 100, NULL, 1, NULL);
 
  vTaskStartScheduler();
  //vTaskDelay(pdMS_TO_TICKS(1000));

}

void loop() {
  // put your main code here, to run repeatedly:

}
