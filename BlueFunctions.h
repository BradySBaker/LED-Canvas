#ifndef BLUE_FUNCTIONS_H
#define BLUE_FUNCTIONS_H

#include <SoftwareSerial.h>
#include "FastLED.h"
#include "colors.h"
#include <string.h>
#include "ledMatrix.h"
#include <SD.h>
#include <SPI.h>

void displayFrame(String dir);
void handleColorChange(char charBuffer[20], bool rain);
void handleBluetooth();
void handleDraw(char charBuffer[20]);
void handleAnimPlay();
void handleFrameSave(char charBuffer[20], bool animation);
void handleFrameDelete(char charBuffer[20]);
void handleAnimDelete(char charBuffer[20]);
int readFileCount(String directory);
void sendFileNames(String dir, bool anims);

void raindrop();

void audioVisualizer();

extern CRGB leds[256];

extern SoftwareSerial bluetooth;

extern byte Colors[3];

extern String playingAnim;

//Raindrop stuff
extern bool raining;
extern int raindropAmount;
extern int curColorLength;
extern COLORS palleteColors[6];
extern String background;

#endif