#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "all_frames.h"

//Display Configs
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

// Animation Sleep
#define SLEEP_START_FRAME 90
#define SLEEP_END_FRAME 132

// Random time sleep
#define SLEEP_TIMEOUT_MIN 10000  // 10s
#define SLEEP_TIMEOUT_MAX 60000  // 60s

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Status
bool isSleep = false;
unsigned long sleepStartTime = 0;
unsigned long sleepDuration = 0;

// Animation Configs
#define FRAME_DELAY 42  // 24FPS

void setup() {
  Serial.begin(115200);
  //       (SCL,SDA) of Display
  Wire.begin(3, 4);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  display.display();

  sleepDuration = randomSleepTimeout();
}

void loop() {
  static unsigned long lastFrameTime = 0;
  static int currentFrame = 0;
  unsigned long currentTime = millis();

  if (isSleep && (currentTime - sleepStartTime >= sleepDuration)) {
    isSleep = false;
    sleepDuration = randomSleepTimeout();
  }

  // Start sleeping from first sleep frame
  if (!isSleep && ( currentFrame - SLEEP_START_FRAME == 0 )) {
    sleepStartTime = currentTime;
    isSleep = true;
  }

    // Draw frame
    if (currentTime - lastFrameTime >= FRAME_DELAY) {
      lastFrameTime = currentTime;
      display.clearDisplay();
      if (isSleep) {
        currentFrame++;
        if (currentFrame > SLEEP_END_FRAME) {
         currentFrame = SLEEP_START_FRAME;
        }
      } else {
        currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
      }
    display.drawBitmap(0, 0, frames[currentFrame], FRAME_WIDTH, FRAME_HEIGHT, SSD1306_WHITE);
  }
  display.display();
}

// Function to generate random sleep time from 10 -> 60 seconds
unsigned long randomSleepTimeout() {
  return random(SLEEP_TIMEOUT_MIN, SLEEP_TIMEOUT_MAX + 1);
}

