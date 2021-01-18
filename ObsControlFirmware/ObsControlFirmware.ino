/* 
 * OBS STUDIO CONTROL RIG version 1.5 (2021-01-14)
 * 
 * Copyright (c) Michal Altair Valasek, 2019-2021
 * Licensed under terms of the MIT License.
 * 
 * https://www.rider.cz
 * https://www.altair.blog
 * https://github.com/ridercz/ObsControl/
 * 
 */

#include <Pushbutton.h>
#include <Keyboard.h>
#include <Adafruit_NeoPixel.h>

// Uncomment the following line to print values to serial instead of sending keypresses
// #define WHATIF

// How long should the key be down?
#define KEYPRESS_LENGTH         50 // ms

// POST LED blinking
#define TEST_LED_COUNT          5
#define TEST_LED_SPEED          100 // ms

// Define pins and count for standard buttons
#define STANDARD_BUTTON_COUNT   5
Pushbutton standardButtons[] {
  Pushbutton(6),                // Green  (Ctrl-Shift-Alt-F1)
  Pushbutton(5),                // Yellow (Ctrl-Shift-Alt-F2)
  Pushbutton(4),                // Black  (Ctrl-Shift-Alt-F3)
  Pushbutton(3),                // Blue   (Ctrl-Shift-Alt-F4)
  Pushbutton(2),                // Red    (Ctrl-Shift-Alt-F5)
};

// Define pins and count for LED status buttons
#define STATUS_BUTTON_COUNT     2
#define STATUS_BUTTON_LED       14
Pushbutton statusButtons[] {
  Pushbutton(8),                // Square red   (Ctrl-Shift-Alt-F6)
  Pushbutton(7),                // Square green (Ctrl-Shift-Alt-F7)
};
bool statusCurrent[STATUS_BUTTON_COUNT];

// Define pin for RGB LED APA106
// Comment the following line to disable the RGB LED functionality
#define RGB_PIN                 16
#ifdef RGB_PIN
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, RGB_PIN, NEO_RGB + NEO_KHZ800);
unsigned long buttonColors[] = {
  0x00ff00,                     // Green
  0xffff00,                     // Yellow
  0xffffff,                     // White (black button)
  0x0000ff,                     // Blue
  0xff0000                      // Red
 };
#endif

void setup() {
#ifdef WHATIF
  while (!Serial);
  Serial.begin(9600);
  delay(500);
  Serial.print("Initializing...");
#endif

#ifdef RGB_PIN
  // Initialize RGB LED
  pixels.begin();
  pixels.fill(0xff0000); pixels.show(); delay(TEST_LED_SPEED * TEST_LED_COUNT); // Red
  pixels.fill(0x00ff00); pixels.show(); delay(TEST_LED_SPEED * TEST_LED_COUNT); // Green
  pixels.fill(0x0000ff); pixels.show(); delay(TEST_LED_SPEED * TEST_LED_COUNT); // Blue
  pixels.fill(0x000000); pixels.show();                                         // Black (off)
#endif

  // Initialize status buttons
  for (int i = 0; i < STATUS_BUTTON_COUNT; i++) {
    statusCurrent[i] = false;
    pinMode(STATUS_BUTTON_LED + i, OUTPUT);
    for(int j = 0; j < TEST_LED_COUNT; j++) {
      digitalWrite(STATUS_BUTTON_LED + i, HIGH);
      delay(TEST_LED_SPEED);
      digitalWrite(STATUS_BUTTON_LED + i, LOW);
      delay(TEST_LED_SPEED);
    }
  }

#ifdef WHATIF
  Serial.println("Done");
#endif
}

void loop() {
  // Initialize standard buttons
  for (int i = 0; i < STANDARD_BUTTON_COUNT; i++) {
    if (standardButtons[i].getSingleDebouncedPress()) pressStandardButton(i);
  }

  // Initialize status buttons
  for (int i = 0; i < STATUS_BUTTON_COUNT; i++) {
    if (statusButtons[i].getSingleDebouncedPress()) pressStatusButton(i);
  }
}

void pressStandardButton(int index) {
#ifdef WHATIF
  Serial.print("Pressed standard button #");
  Serial.println(index + 1);
#else
  sendFunctionKey(index);
#endif
#ifdef RGB_PIN
  pixels.fill(buttonColors[index]);
  pixels.show();
#endif
}

void pressStatusButton(int index) {
  statusCurrent[index] = !statusCurrent[index];
  digitalWrite(STATUS_BUTTON_LED + index, statusCurrent[index]);
#ifdef WHATIF
  Serial.print("Status button #");
  Serial.print(index + 1);
  Serial.println(statusCurrent[index] ? " enabled" : " disabled");
#else
  sendFunctionKey(index + STANDARD_BUTTON_COUNT);
#endif
}

void sendFunctionKey(int index) {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_F1 + index);
  delay(KEYPRESS_LENGTH);
  Keyboard.releaseAll();
}
