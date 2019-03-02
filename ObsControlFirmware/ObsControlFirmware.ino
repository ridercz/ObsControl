#include <Pushbutton.h>
#include <Keyboard.h>

// Uncomment the following line to print values to serial instead of sending keypresses
// #define WHATIF
#define KEYPRESS_LENGTH         50

// Define pins and count for standard buttons
#define STANDARD_BUTTON_COUNT   5
Pushbutton standardButtons[] {
  Pushbutton(6),                // Green
  Pushbutton(5),                // Yellow
  Pushbutton(4),                // Black
  Pushbutton(3),                // Blue
  Pushbutton(2),                // Red
};

// Define pins and count for LED status buttons
#define STATUS_BUTTON_COUNT     2
#define STATUS_BUTTON_LED       14
Pushbutton statusButtons[] {
  Pushbutton(8),                // Square red
  Pushbutton(7),                // Square green
};
bool statusCurrent[STATUS_BUTTON_COUNT];

void setup() {
#ifdef WHATIF
  while (!Serial);
  Serial.begin(9600);
  delay(500);
  Serial.print("Initializing...");
#endif

  // Initialize status buttons
  for (int i = 0; i < STATUS_BUTTON_COUNT; i++) {
    statusCurrent[i] = false;
    pinMode(STATUS_BUTTON_LED + i, OUTPUT);
    digitalWrite(STATUS_BUTTON_LED + i, false);
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
