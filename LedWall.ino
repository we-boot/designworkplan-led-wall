#include <FastLED.h>

#define WIDTH 8
#define HEIGHT 8
#define LED_COUNT WIDTH * HEIGHT
#define BRIGHTNESS 30
#define LED_PIN 3
#define COLOR_ORDER GRB
#define CHIPSET WS2811

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

// Generated with https://javl.github.io/image2cpp/
// Make sure to generate with the following settings:
// - Canvas size: 16 x 16 (according to size of led matrix)
// - Background color: black
// - Scaling: stretch to fill canvas
// - Code output format: Arduino code, single bitmap
// - Draw mode: 3 bytes per pixel
// MAKE SURE TO REMOVE 'PROGMEM'
#define SPEAKER_WIDTH 8
#define SPEAKER_HEIGHT 8
const uint32_t speaker [] = {
  // 'voice, 8x8px
  0x00000001, 0x00010101, 0x00010100, 0x00000101, 0x00000101, 0x00cdd7df, 0x00000001, 0x00010000,
  0x00000101, 0x00000000, 0x00000100, 0x00010000, 0x00cdd7df, 0x00ccd7de, 0x00010101, 0x00000100,
  0x00010101, 0x00000001, 0x00889ba6, 0x00cdd7df, 0x00cdd7df, 0x00ccd7df, 0x00010001, 0x00010001,
  0x00010101, 0x00010101, 0x00889ba7, 0x00cdd7df, 0x00cdd6de, 0x00cdd6de, 0x00000000, 0x00000101,
  0x00010000, 0x00010000, 0x00889aa6, 0x00cdd7df, 0x00ccd6de, 0x00ccd6df, 0x00000001, 0x00000000,
  0x00000001, 0x00000100, 0x00889aa6, 0x00ccd6de, 0x00ccd7de, 0x00cdd7df, 0x00010001, 0x00010000,
  0x00010000, 0x00000101, 0x00010001, 0x00000100, 0x00ccd6df, 0x00ccd7de, 0x00000001, 0x00000001,
  0x00000000, 0x00000000, 0x00000101, 0x00010001, 0x00010001, 0x00cdd6df, 0x00010100, 0x00010101
};

#define YOUTUBE_WIDTH 8
#define YOUTUBE_HEIGHT 8
const uint32_t youtube[] = {
  // '1384060, 8x8px
  0x00010000, 0x00010000, 0x00000101, 0x00010001, 0x00000000, 0x00010000, 0x00000100, 0x00010101, 
  0x00000000, 0x00ff0001, 0x00fe0000, 0x00fe0000, 0x00fe0000, 0x00ff0100, 0x00ff0101, 0x00010100, 
  0x00fe0100, 0x00fe0100, 0x00ff0000, 0x00fffefe, 0x00ff0001, 0x00fe0101, 0x00fe0101, 0x00fe0100, 
  0x00fe0101, 0x00ff0101, 0x00ff0000, 0x00fffefe, 0x00fffefe, 0x00ff0100, 0x00ff0100, 0x00ff0100, 
  0x00ff0101, 0x00fe0001, 0x00fe0001, 0x00ffffff, 0x00ffffff, 0x00ff0101, 0x00ff0000, 0x00fe0000, 
  0x00fe0001, 0x00fe0101, 0x00ff0101, 0x00ffffff, 0x00fe0000, 0x00fe0100, 0x00fe0001, 0x00fe0001, 
  0x00010000, 0x00ff0000, 0x00fe0100, 0x00fe0100, 0x00ff0000, 0x00fe0001, 0x00ff0000, 0x00000100, 
  0x00010100, 0x00010001, 0x00010000, 0x00010000, 0x00010101, 0x00000101, 0x00000000, 0x00000000
};


void display_image(uint32_t* picture, int startX, int startY, int width, int height) {
  for (int y = 0; y < height; y++) {
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

      uint32_t color = picture[y * width + x];
      leds[i] = CRGB((color >> 16) & 0xFF, (color >> 8) & 0xFF, (color >> 0) & 0xFF);
    }
  }
}

void display_binary_image(uint16_t* picture, int startX, int startY, int width, int height, CRGB color) {
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

uint8_t current_animation = 1;
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
    display_binary_image(arrow, hsvCounter % 24 - 8, 0, 8, 8, c);
    display_binary_image(arrow, (hsvCounter + 12) % 24 - 8, 0, 8, 8, c);
  }
  else if (current_animation == 1) {
    clear_display();
    display_image(youtube, 0, 0, YOUTUBE_WIDTH, YOUTUBE_HEIGHT);
  }
  else if (current_animation == 2) {
    clear_display();
    display_image(speaker, 0, 0, SPEAKER_WIDTH, SPEAKER_HEIGHT);
  }

  FastLED.show();
  delay(100);
  hsvCounter++;
}
