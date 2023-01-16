
#include <UTFT.h>
#include <Touch.h>
#include <MapleFreeRTOS900.h>

UTFT myGLCD(ILI9325D_16, PD11, PD5, PD7, PC1);
Touch touch(PA6, PB6, PA5, PB7, PA7);

typedef struct Point {
  uint16_t x;
  uint16_t y;
} point_t;

typedef struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} rgb_t;

class Button {
public:
  uint16_t m_square_position[4];
  rgb_t m_color;
  char m_color_name[20];
public:
  Button(uint16_t quare_position[4], rgb_t color, const char *color_name)
      : m_point{ point }, m_color{ color }  {
    strncpy(m_color_name, color_name, strlen(color_name));
  }

  bool isPressed(int x, int y) {
    
  }
};

void createColoredButtons() {
  for (int i = 0; i < 8; i++) {
    
  }
}

void setup() {
  // put your setup code here, to run once:
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);

  touch.init(320,240,3900,3900);


  xTaskCreate(task1, "Pixel", 100, NULL,1,NULL);
  xTaskCreate(task2, "touchPixel", 100, NULL,1,NULL);
  vTaskStartScheduler();
}
