#include <cstdint>
#include <cstring>
#include <cstdio>

UTFT myLcd(ILI9325D_16, PD11, PD5, PD7, PC1);
Touch ttouch(PA6, PB6, PA5, PB7, PA7);

extern uint8_t SmallFont[];


#define NUMBER_COLOR_BUTTONS 8
#define NUMBER_MENU_BUTTONS 3

enum draw_status {
  START,
  CLEAR,
  STOP
};

enum draw_status menu;

typedef struct Point {
  uint16_t x;
  uint16_t y;
} point_t;
  
typedef struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} rgb_t;

point_t current_position;
rgb_t current_color;

class Button {
public:
  uint16_t m_square_position[4];
  rgb_t m_color;
  char m_text[10];
public:
 
  void setData(const uint16_t * square_position, rgb_t color, const char * text) {
    for (int i = 0; i < 4; i++) m_square_position[i] = square_position[i];
    m_color = color;
    strncpy(m_text, text, strlen(text));

    if (strlen(text) == 0) {
     myLcd.fillRect(
         m_square_position[0],
         m_square_position[1],
         m_square_position[2],
         m_square_position[3]);

     myLcd.setColor(
         m_color.r,
         m_color.g,
         m_color.b);
    }
    else {
      myLcd.drawRect(
          m_square_position[0],
          m_square_position[1],
          m_square_position[2],
          m_square_position[3]);
    }
  }
 

  bool isPressed(uint16_t pressed_x, uint16_t pressed_y) {
    uint16_t x = m_square_position[0];
    uint16_t y = m_square_position[1];
    uint16_t w = m_square_position[2];
    uint16_t h = m_square_position[3];

    if (x < pressed_x && pressed_x < w &&
        y < pressed_y && pressed_y < h)
      return true;

    return false;
  }
};


Button colored_buttons[NUMBER_COLOR_BUTTONS];
Button menu_buttons[NUMBER_MENU_BUTTONS];

rgb_t colors[NUMBER_COLOR_BUTTONS] = {
  {0, 0, 0},      // black
  {0, 0, 139},    // dark blue
  {0, 255, 255},  // cyan
  {255, 0, 0},    // red
  {255, 0, 255},  // magenta
  {255, 255, 0},  // yellow
  {0, 255, 0},    // green
  {255, 255, 255} // white
};


void debug() {
  for (int i = 0; i < NUMBER_COLOR_BUTTONS; i++) {

    std::printf("Button %d: \n", i + 1);
    std::printf("square_pos = { %d, %d, %d, %d }\n",
                colored_buttons[i].m_square_position[0],
                colored_buttons[i].m_square_position[1],
                colored_buttons[i].m_square_position[2],
                colored_buttons[i].m_square_position[3]);


    std::printf("color= { %d, %d, %d }\n",
                colored_buttons[i].m_color.r,
                colored_buttons[i].m_color.g,
                colored_buttons[i].m_color.b);
    std::puts("");
  }
}



void createButtons() {

  const char * menu_button_text[] = { "Start", "Clear", "Stop" };

  uint16_t button_width =  myLcd.getDisplayXSize() / 8;


  // starting points for the first button
  // x = start inc for 20 for every next button

  uint16_t x = 0; 


  // create colored buttons
  for (int i = 0; i < NUMBER_COLOR_BUTTONS; i++) {
    uint16_t pos[4];
    pos[0] = x;
    pos[1] = 0;
    pos[2] = button_width;
    pos[3] = 20;

    colored_buttons[i].setData(pos, colors[i], "");
    x += button_width;
  }

  x = 0;
  button_width = myLcd.getDisplayXSize() / 3;

  uint16_t text_position_y = 230; // middle of menu button
  // create menu buttons
  for (int i = 0; i < NUMBER_MENU_BUTTONS; i++) {
    uint16_t pos[4];
    pos[0] = x;
    pos[1] = 220;
    pos[2] = button_width;
    pos[3] = 239;

    menu_buttons[i].setData(pos, colors[7], menu_button_text[i]); // white color

    myLcd.print(menu_button_text[i], x + 25, text_position_y);
    x += button_width;
  }
}


void Task1(void *pvParameters) {

  rbt_t old_color = current_color;
  rbt_t new_color = {0, 0, 0};

  uint16_t current_x = -1;
  uint16_t current_y = -1;

  for (;;) {
    new_color = current_color;
    if (memcmp(old_color, new_color) != 0) {
      myLcd.setColor(
          new_color.r,
          new_color.g,
          new_color.b);

      if (menu == START) {
        current_x = 2;
        current_y = 30;
        menu = -1;
      }
      else if (menu == CLEAR) {
        current_x = 2;
        current_y = 30;
        menu = -1;
      }
      else if (menu == STOP) 
        continue;

      if (current_x == myLcd.getDisplayXSize() - 2) {
        current_x = 0;
        current_y += 5:
      }
      else if (current_y == myLcd.getDisplayYSize() - 2) {
        current_x = 2;
        current_y += 32:
      }

      current_x++;
      myLcd.drawPixel(current_x, current_y);
      vTaskDelay(pdMS_TO_TICKS(10));

    }
    
    old_color = new_color;
  }
  
}

void Task2(void *pvParameters) {

  for (;;) {

    while (ttouch.touchEvent() == 0) {
      vTaskDelay(pdMS_TO_TICKS(10));
    }

    p = ttouch.readTouch();

    for (int i = 0; i < NUMBER_COLOR_BUTTONS; i++) {
      if (colored_buttons[i].isPressed(p.x, p.y)) {
        current_color = colored_buttons[i].m_color;
      }
    }
    for (int i = 0; i < NUMBER_MENU_BUTTONS; i++) {
      if (menu_buttons[i].isPressed(p.x, p.y)) {
        if (strcmp(menu_buttons[i].m_text, "Start") == 0)
          menu = START;
        else if (strcmp(menu_buttons[i].m_text, "Clear") == 0)
          menu = CLEAR;
        else if (strcmp(menu_buttons[i].m_text, "Stop") == 0)
          menu = STOP;
      }
    }
  }
}


void setup() {

  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);


  ttouch.init(320, 240, 3900, 3900);


  createButtons();


  xTaskCreate(Task1, "Pixel", 100, NULL, 2, NULL);
  xTaskCreate(Task2, "touch", 100, NULL, 2, NULL);
  vTaskStartScheduler();


}

void loop() {

}

