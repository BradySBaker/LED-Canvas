#include "BlueFunctions.h"

#define LEDPIN 7
#define button 5
const int chipSelect = 53;

CRGB leds[256];

SoftwareSerial bluetooth(10, 11);

COLORS Colors(255, 0, 0);

String playingAnim = "~";

COLORS hexToRGB(const char* hexColorString) {
    COLORS result;
    unsigned long colorInt = 0;
    for (int i = 0; i < 6; i++) {
    if (hexColorString[i] < 97) { //number parts
      colorInt = colorInt + (((long)hexColorString[i] - 48) << ((5 - i) * 4));
    } else { //letter in hex parts
      colorInt = colorInt + (((long)hexColorString[i] - 97  + 10) << ((5 - i) * 4));
    }
  }
  result.r = (colorInt >> 16) & 0xFF;
  result.g = (colorInt >> 8) & 0xFF;
  result.b = colorInt & 0xFF;
  return result;
};

COLORS paletteColors[6] = {};

// Raindrop Variables
bool raining = false;
int raindropAmount = 5;
int curColorLength = 0;
String background = "~";

// AV variables
bool avActive = false;

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
