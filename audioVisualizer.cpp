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

COLORS interpolateColor(const COLORS& startColor, const COLORS& endColor, float progress) {
  COLORS result;

  result.r = startColor.r + static_cast<byte>((endColor.r - startColor.r) * progress);
  result.g = startColor.g + static_cast<byte>((endColor.g - startColor.g) * progress);
  result.b = startColor.b + static_cast<byte>((endColor.b - startColor.b) * progress);
  return result;
}

void resetPixels() {
  for (int i = 0; i < 16; i++) {
    fallingPixels[i] = -1; //y
  }
  fallingPixels[1] = 15;
}

void drawFallingPixel(int barHeight, int x) {
  if (barHeight > fallingPixels[x]) {
    fallingPixels[x] = barHeight;
  } else {
    fallingPixels[x]--;
  }
  if (fallingPixels[x] > 0) {
    const int fallPos =  pgm_read_word(&LED_MAP[15-fallingPixels[x]][x]);
    leds[fallPos].r = paletteColors[curColorLength].r;
    leds[fallPos].g = paletteColors[curColorLength].g;
    leds[fallPos].b = paletteColors[curColorLength].b;
  }
};

void audioVisualizer(bool pixelFall) {
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ));
  resetPixels();
  while(avActive) {
    handleBluetooth();
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
      for (byte x = 0; x < 16; x++) {
        // Scale the bars for the display
        int barHeight = bandValues[x] / AMPLITUDE;
        for (byte y = 0; y < barHeight; y++) {
          const int curPos = pgm_read_word(&LED_MAP[15-y][x]);
          float progress = static_cast<float>(y) / static_cast<float>(barHeight);
          progress *= progress;
          COLORS color = interpolateColor(paletteColors[0], paletteColors[1], progress);
          leds[curPos].r = color.r;
          leds[curPos].g = color.g;
          leds[curPos].b = color.b;
        }
        if (pixelFall) {
          drawFallingPixel(barHeight, x);
        }
      }

    FastLED.show();
  }
}