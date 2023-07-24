#include "BlueFunctions.h"

#define LEDPIN 7
#define button 5
const int chipSelect = 53;

CRGB leds[256];

SoftwareSerial bluetooth(10, 11);

byte Colors[3] = {100, 0, 0};

String playingAnim = "~";

// Raindrop Variables
bool raining = false;
int raindropAmount = 5;
int curColorLength = 0;
COLORS palleteColors[6] = {};
String background = "~";

// Audio Visualizer Variables

void setup() {
  bluetooth.begin(115200);
  Serial.begin(115200);

  FastLED.addLeds<WS2812B, LEDPIN, GRB>(leds, 256);

  FastLED.setBrightness(100);

  randomSeed(analogRead(0));
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
  }
  // bluetooth.print("AT+NAMELedPanel");
  pinMode(button,INPUT_PULLUP);
}

void loop() {
  handleBluetooth();
  if (playingAnim != "~") {
    handleAnimPlay();
  }
  if (!digitalRead(button)) {
    // Serial.println("Pushed button");
    bluetooth.print("Pushed Button");
    while(!digitalRead(button)) {}
  }
}
