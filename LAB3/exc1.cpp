
#include <MapleFreeRTOS900.h>


#define LED_GREEN PB0
#define LED_YELLOW PB1
#define LED_RED PB2

#define SWITCH PC0

#define VAR_RESISTOR PA0;

volatile int LED_GREEN_TIME_MS = 2000;
volatile int LED_YELLOW_TIME_MS = 500;
volatile int LED_RED_TIME_MS = 3000;


volatile int counter = 0;

void incrementCounter() {
  counter++;
}

void vTaskCounter(void *pvParameters) {

  for (;;) {
    Serial.print("\ncounter: ");
    Serial.println(counter);
    vTaskDelay(pdMS_TO_TICKS(750)); 
  }
}

void vTaskTrafficLight(void *pvParameters) {

    for (;;) {
      // GREEN
      digitalWrite(LED_GREEN, LOW);
      vTaskDelay(pdMS_TO_TICKS(LED_GREEN_TIME_MS));
      digitalWrite(LED_GREEN, HIGH);
      // YELLOW
      digitalWrite(LED_YELLOW, LOW);
      vTaskDelay(pdMS_TO_TICKS(LED_YELLOW_TIME_MS));
      digitalWrite(LED_YELLOW, HIGH);
      // RED AND RED / YELLOW
      digitalWrite(LED_RED, LOW);
      vTaskDelay(pdMS_TO_TICKS(LED_RED_TIME_MS));

      digitalWrite(LED_YELLOW, LOW);
      vTaskDelay(pdMS_TO_TICKS(LED_YELLOW_TIME_MS));
      
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, HIGH);
    }
}

void vTaskAnalogRead(void *pvParameters) {

  for (;;) {
    int var_res = analogRead(VAR_RESISTOR);
    if (var_res < 1024) {
      LED_GREEN_TIME_MS = 2000;
      LED_YELLOW_TIME_MS = 500;
      LED_RED_TIME_MS = 3000;
    } else if (1024 <= var_res && var_res < 2048) {
      LED_GREEN_TIME_MS = 3000;
      LED_YELLOW_TIME_MS = 500;
      LED_RED_TIME_MS = 4000;
    } else if (2048 <= var_res && var_res < 3050) {
      LED_GREEN_TIME_MS = 4000;
      LED_YELLOW_TIME_MS = 500;
      LED_RED_TIME_MS = 5000;
    } else if (3050 <= var_res) {
      LED_GREEN_TIME_MS = 5000;
      LED_YELLOW_TIME_MS = 500;
      LED_RED_TIME_MS = 6000;
    }
    
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {

  Serial.begin(9600);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);
  
  

  attachInterrupt(SWITCH, incrementCounter, FALLING);

  xTaskCreate(vTaskCounter, "Task Counter", 100, NULL, 1, NULL);
  xTaskCreate(vTaskTrafficLight, "Task Name Traffic", 100, NULL, 1, NULL);
  xTaskCreate(vTaskAnalogRead, "Task AnalogRead", 100, NULL, 1, NULL);
 
  vTaskStartScheduler();
}

void loop() {
  // nothing
}
