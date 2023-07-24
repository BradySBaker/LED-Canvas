#include "BlueFunctions.h"

void displayFrame(String dir) {
  // Open the colors file on the SD card
  File file = SD.open(dir);
  if (!file) {
    Serial.println("File open failed!");
      bluetooth.print("failed");
    return;
  }
  
  // Loop through all the pixels on the LED panel
  for (int i = 0; i < 256; i++) {
    // Read the color from the file as a 32-bit integer
    uint32_t color;
    file.read((uint8_t*)&color, sizeof(color));
    // Extract the red, green, and blue components from the color
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    // Set the pixel color on the LED panel
    leds[i].r = r;
    leds[i].g = g;
    leds[i].b = b;
  }
  
  // Close the colors file
  file.close();
  // Update the LED panel with the new colors
  FastLED.show();
  bluetooth.print("successful");
}


void handleDraw(char charBuffer[20]) {
  // Serial.println(charBuffer);
  String positions[3] = {};
  int idx = 0;
  char* charPos = strtok(charBuffer, "P");
  while (charPos != NULL && idx != 3) {
    positions[idx] = charPos;
    charPos = strtok(NULL, "P");
    idx++;
  }
  for (int i = 0; i < idx; i++) {
    int commaPos = positions[i].indexOf(",");
    String x = positions[i].substring(0, commaPos);
    String y = positions[i].substring(commaPos + 1);

    int curPos = pgm_read_word(&LED_MAP[x.toInt()][y.toInt()]);
    leds[curPos].r = Colors[0];
    leds[curPos].g = Colors[1];
    leds[curPos].b = Colors[2];
  }
  FastLED.show();
  bluetooth.print("successful");
}


void handleAnimPlay() {
  String dirName = "/anims/" + playingAnim + "/";

  Serial.println("Attempting to play animation from" + dirName);

  int result = 0;
  File dir = SD.open(dirName);
  while (true) {
    File file = dir.openNextFile();
    if (!file) {
      break; // No more files
    }
    result++;
    displayFrame(dirName + String(result) + ".TXT");
    Serial.println("Loading frame " + String(result));
    file.close();
  }
  dir.close();
}


void handleColorChange(char charBuffer[20], bool rain) {
  char hexColorString[10] = "";
  if (rain) {
      strcpy(hexColorString, &charBuffer[3]); //3 = crr
      bluetooth.print("cRAIN");
  } else {
      strcpy(hexColorString, &charBuffer[2]); //2 = cr
        bluetooth.print("successful");
  }
  unsigned long colorInt = 0;
  //http://www.asciitable.com
  for (int i = 0; i < 6; i++) {
    if (hexColorString[i] < 97) { //number parts
      colorInt = colorInt + (((long)hexColorString[i] - 48) << ((5 - i) * 4));
    } else { //letter in hex parts
      colorInt = colorInt + (((long)hexColorString[i] - 97  + 10) << ((5 - i) * 4));
    }
  }
  byte redColor = (colorInt >> 16) & 0xFF;
  byte greenColor = (colorInt >> 8) & 0xFF;
  byte blueColor = colorInt & 0xFF;
  if (!rain) {
    Colors[0] = redColor;
    Colors[1] = greenColor;
    Colors[2] = blueColor;
  } else {
    palleteColors[curColorLength] = {redColor, greenColor, blueColor};
    curColorLength++;
  }
}