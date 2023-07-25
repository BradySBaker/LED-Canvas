#include "BlueFunctions.h"

void handleBluetooth() {
  char charBuffer[20];

  if (bluetooth.available() > 0) { 
    int numberOfBytesReceived = 0;
    // if (!game) {
    numberOfBytesReceived = bluetooth.readBytesUntil('\r', charBuffer, 19);
    // } else {
    //   numberOfBytesReceived = bluetooth.readBytesUntil('\r', charBuffer, 2);
    // }
    charBuffer[numberOfBytesReceived] = NULL;
    Serial.println(charBuffer);
   if(strstr(charBuffer, "OFF") == &charBuffer[0]) { //Handle off signal
      FastLED.clear();
      FastLED.show();
      playingAnim = "~";
      bluetooth.print("OFF");
    } else if (strstr(charBuffer, "R") == &charBuffer[0]) { //Handle rain signal
      raining = true;
      char numString[3] = "";
      strcpy(numString, &charBuffer[1]);
      raindropAmount = atoi(numString);
      bluetooth.print("RAIN");
      raindrop();
    } else if (strstr(charBuffer, "SM") == &charBuffer[0]) { //Handle stop rain/audio visualizer
      Serial.println("Stopped mode");
      bluetooth.print("SM");
      curColorLength = 0;
      raining = false;
      avActive = false;
    } else if(strstr(charBuffer, "CM") == &charBuffer[0]) { //Handle rain/audio visualizer color added
      handleColorChange(charBuffer, true);
    } else if (strstr(charBuffer, "C") == &charBuffer[0]) { //Handle color change
      handleColorChange(charBuffer, false);
    } else if (strstr(charBuffer, "P") == &charBuffer[0]) { //Handle pixel drawing
      handleDraw(charBuffer);
    } else if (strstr(charBuffer, "HAV") == &charBuffer[0]) { //Handle audio visualizer
      avActive = true;
      bluetooth.print("HAV");
      audioVisualizer();
    } else if (strstr(charBuffer, "S") == &charBuffer[0]) { //Handle frame save
      handleFrameSave(charBuffer, false);
    } else if (strstr(charBuffer, "F") == &charBuffer[0]) { //Handle display frame
      char nameString[7] = "";
      strcpy(nameString, &charBuffer[1]);
      if (raining) {
        background = nameString;
        bluetooth.print("FRAME");
      } else {
        displayFrame("/drawings/" + String(nameString) + ".TXT");
      }
    } else if (strstr(charBuffer, "names") == &charBuffer[0]) { //Handle retrieve frame ammount
      sendFileNames("/drawings", false);
      sendFileNames("/ANIMS", true);
      bluetooth.print("~");
    } else if (strstr(charBuffer, "D") == &charBuffer[0]) { //Handle frame delete
      handleFrameDelete(charBuffer);
    } else if (strstr(charBuffer, "A") == &charBuffer[0]) { //Handle frame save
      handleFrameSave(charBuffer, true);
    } else if (strstr(charBuffer, "I") == &charBuffer[0]) { //Handle animation play
     char nameString[7] = "";
     strcpy(nameString, &charBuffer[1]);
     playingAnim = String(nameString);
     bluetooth.print("successful");
    } else if (strstr(charBuffer, "Z") == &charBuffer[0]) { //Handle animation delete
     handleAnimDelete(charBuffer);
    }
  }
}