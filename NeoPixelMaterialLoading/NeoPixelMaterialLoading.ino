#include <Adafruit_NeoPixel.h>
#include <Ramp.h>
ramp c;
ramp f;
ramp j;

#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN            6
#define NUMPIXELS      24
#define LOOP_DURATION 1666
#define CENTER_DURATION 2000

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

char frame[NUMPIXELS];

void setup() {
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  pixels.begin(); // This initializes the NeoPixel library.
  c.go(NUMPIXELS, CENTER_DURATION, LINEAR, LOOPFORWARD);
  f.go(NUMPIXELS, LOOP_DURATION, CUBIC_IN, LOOPFORWARD);
  j.go(NUMPIXELS, LOOP_DURATION, CUBIC_OUT, LOOPFORWARD);
}

void loop() {
  for (int i = 0 ; i < NUMPIXELS; i++) {
    if (f.value() <= i && i <= j.value()) {
      frame[i] = 1;
    } else {
      frame[i] = 0;
    }
  }
  for (int i = 0 ; i < NUMPIXELS; i++) {
    int k = i+c.value();
    if(NUMPIXELS <= k){
      k = k-NUMPIXELS;
    }
    if (frame[i] == 1) {
      pixels.setPixelColor(k, pixels.Color(49, 0, 119));
    } else {
      pixels.setPixelColor(k, pixels.Color(0, 0, 0));
    }
  }

  pixels.show();
  c.update();
  f.update();
  j.update();
  delay(10);
}


