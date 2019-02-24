#include <Adafruit_NeoPixel.h>
#include <Ramp.h>
ramp c;
ramp f;
ramp j;

#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN            6
#define POT_PIN     A1
#define NUMPIXELS      24
#define LOOP_DURATION 1666
#define CENTER_DURATION 2500


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

  Serial.begin(9600);
}

void loop() {

  int hue = analogRead(POT_PIN);
  hue = map(hue, 0, 1023, 0, 359);
  int r, g, b;
  if ( hue < 61) {
    r = 255;
    g = (hue * 255) / 60 ;
    b = 0;
  } else if (hue < 121) {
    r = ((120 - hue) * 255) / 60 ;
    g = 255;
    b = 0;
  } else if (hue < 181) {
    r = 0;
    g = 255;
    b = ((hue - 120) * 255) / 60;
  } else if (hue < 241) {
    r = 0;
    g = ((240 - hue) * 255) / 60;
    b = 255;
  } else if (hue < 301) {
    r = ((hue - 240) * 255) / 60;
    g = 0;
    b = 255;
  } else {
    r = 255;
    g = 0;
    b = ((360 - hue) * 255) / 60 ;
  }

  Serial.println("---");
  Serial.println(hue);
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);

  for (int i = 0 ; i < NUMPIXELS; i++) {
    if (f.value() <= i && i <= j.value()) {
      frame[i] = 1;
    } else {
      frame[i] = 0;
    }
  }
  for (int i = 0 ; i < NUMPIXELS; i++) {
    int k = i + c.value();
    if (NUMPIXELS <= k) {
      k = k - NUMPIXELS;
    }
    if (frame[i] == 1) {
      pixels.setPixelColor(k, pixels.Color(r, g, b));
    } else {
      pixels.setPixelColor(k, pixels.Color(0, 0, 0));
    }
  }

  pixels.show();
  c.update();
  f.update();
  j.update();
  delay(15);
}


