#include "BlueFunctions.h"
#define micPin A1

#include <arduinoFFT.h>

#define DATA_PIN 7
#define SAMPLES 64
#define SAMPLING_FREQ   10000
unsigned long newTime;
#define AMPLITUDE 50
int bandValues[16];
double vReal[SAMPLES];
double vImag[SAMPLES];
#define NOISE        100
unsigned int sampling_period_us;
arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);

char x = 0, ylim = 16;
int i = 0, val;

int fallingPixels[16];



void resetPixels() {
  for (int i = 0; i < 16; i++) {
    fallingPixels[i] = -1; //y
  }
  fallingPixels[1] = 15;
}

void audioVisualizer() {
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ));
  resetPixels();
  while(true) {
    for (int i = 0; i<16; i++){
      bandValues[i] = 0;
    }
    FastLED.show();
    for (int i = 0; i < SAMPLES; i++) {
      newTime = micros();
      vReal[i] = analogRead(micPin); // A conversion takes about 9.7uS on an ESP32
      vImag[i] = 0;
      while ((micros() - newTime) < sampling_period_us) { /* chill */ }
    }

    FFT.DCRemoval();
    FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(FFT_FORWARD);
    FFT.ComplexToMagnitude();

    int frequencyBins[] = {2, 3, 4, 6, 8, 10, 12, 14, 16, 17, 19, 21, 23, 26, 28, SAMPLES / 2};

    for (int i = 0; i < 16; i++) {
      bandValues[i] = 0;
    }

    // ... (existing code)

    for (int i = 2; i < (SAMPLES / 2); i++) {
      if (vReal[i] > NOISE) {
        for (int band = 0; band < 16; band++) {
          if (i <= frequencyBins[band]) {
            bandValues[band] += (int)vReal[i];
            break; // Exit the loop after adding the value to the corresponding band
          }
        }
      }
    }
    FastLED.clear();
      for (byte x = 0; x < 16;x++) {
        // Scale the bars for the display
        int barHeight = bandValues[x] / AMPLITUDE;
        for (byte y = 0; y < barHeight; y++) {
          const int curPos = pgm_read_word(&LED_MAP[x][15-y]);
          leds[curPos].r = 255/(y+1);
          leds[curPos].g = 0;
          leds[curPos].b = ((y+1) * 16) - 1;
        }
        if (barHeight > fallingPixels[x]) {
          fallingPixels[x] = barHeight;
        } else {
          fallingPixels[x]--;
        }
        if (fallingPixels[x] > 0) {
          const int fallPos =  pgm_read_word(&LED_MAP[x][15-fallingPixels[x]]);
          leds[fallPos].r = 0;
          leds[fallPos].g = 255;
          leds[fallPos].b = 0;
        }
      }

    FastLED.show();
  }
}