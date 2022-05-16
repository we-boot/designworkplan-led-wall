#include <FastLED.h>

#define WIDTH 8
#define HEIGHT 8
#define LED_COUNT WIDTH * HEIGHT
#define BRIGHTNESS 30
#define LED_PIN 3
#define COLOR_ORDER GRB
#define CHIPSET WS2811

uint16_t youtube[] = {
  0b01111110,
  0b11111111,
  0b11000111,
  0b11000011,
  0b11000011,
  0b11000111,
  0b11111111,
  0b01111110,
};

uint16_t youtube_play[] = {
  0b00000000,
  0b00000000,
  0b00111000,
  0b00111100,
  0b00111100,
  0b00111000,
  0b00000000,
  0b00000000,
};

uint16_t arrow[] = {
  0b00011100,
  0b00111000,
  0b01110000,
  0b11111111,
  0b11111111,
  0b01110000,
  0b00111000,
  0b00011100,
};

CRGB leds[LED_COUNT];

void display_image(uint16_t* picture, int startX, int startY, int width, int height, CRGB color) {
  for (int y = 0; y < height; y++) {
    uint16_t row = picture[y];
    for (int x = 0; x < width; x++) {

      int dx = startX + x;
      int dy = startY + y;

      if (dx < 0 || dy < 0 || dx >= WIDTH || dy >= HEIGHT) {
        continue;
      }

      int i;
      if (dy & 1) {
        // Odd row
        i = dy * WIDTH + (WIDTH - dx - 1);
      }
      else {
        // Even row
        i = dy * WIDTH + dx;
      }

      //      leds[i] = ((row >> x) & 0x1) ? color : CRGB(0, 0, 0);
      if ((row >> x) & 0x1)
        leds[i] = color;
    }
  }
}

void clear_display() {
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
}

void setup() {
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);


  Serial.begin(9600);

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, LED_COUNT);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.println("Starting...\n");
  delay(1000);

  clear_display();
  FastLED.show();

  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CRGB(255, 0, 0);
    FastLED.show();
    delay(10);
  }
}

uint8_t current_animation = 2;
uint8_t hsvCounter = 0;

void loop() {

  if (digitalRead(10) == 0) {
    current_animation = 0;
  }
  else if (digitalRead(11) == 0) {
    current_animation = 1;
  }
  else if (digitalRead(12) == 0) {
    current_animation = 2;
  }

  if (current_animation == 0) {
    clear_display();
    CHSV c = CHSV(hsvCounter * 4, 255, 255);
    display_image(arrow, hsvCounter % 24 - 8, 0, 8, 8, c);
    display_image(arrow, (hsvCounter + 12) % 24 - 8, 0, 8, 8, c);
  }
  else if (current_animation == 1) {
    clear_display();
    display_image(youtube, 0, 0, 8, 8, CRGB(255, 0, 0));
    display_image(youtube_play, 0, 0, 8, 8, CRGB(255, 255, 255));
  }
  else if (current_animation == 2) {
    clear_display();
    display_image(arrow, 0, 0, 8, 8, CRGB(0, 255, 0));
  }

  FastLED.show();
  delay(100);
  hsvCounter++;
}
