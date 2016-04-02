#define PIN 9

#define trigPin 13
#define echoPin 12

#define FADESPEED 10

//#define __PROG_TYPES_COMPAT__
//#include <avr/pgmspace.h>
//#define CIELPWM(a) (pgm_read_word_near(CIEL8 + a)) // CIE Lightness loopup table

int val[] = {
  2,
16,
32,
48,
64,
80,
112,
144,
192,
240,
288,
352,
432,
512,
608,
704,
816,
928,
1072,
1216,
1376,
1536,
1728,
1920,
2144,
2368,
2608,
2880,
3152,
3456,
3760,
4080
 };

/* Configure digital pins 9 and 10 as 16-bit PWM outputs. */
void setupPWM16() {
    DDRB |= _BV(PB1) | _BV(PB2);        /* set pins as outputs */
    TCCR1A = _BV(COM1A1) | _BV(COM1B1)  /* non-inverting PWM */
        | _BV(WGM11);                   /* mode 14: fast PWM, TOP=ICR1 */
    TCCR1B = _BV(WGM13) | _BV(WGM12)
        | _BV(CS10);                    /* no prescaling */
//    ICR1 = 0xffff;                      /* TOP counter value */
    ICR1 = 4095;
}

/* 16-bit version of analogWrite(). Works only on pins 9 and 10. */
void analogWrite16(uint8_t pin, uint16_t val)
{
    switch (pin) {
        case  9: OCR1A = val; break;
        case 10: OCR1B = val; break;
    }
}

int brightness = 0;    // initial brightness of LED
int fadeAmount = 1;

void setup() {
  Serial.begin(9600);
  pinMode(PIN, OUTPUT);
  setupPWM16();
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  
  long duration, distance, factor;
  
  
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5); // Added this line
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  
  factor = 1.0 / distance * 20000;
  
  Serial.println(distance);
  
  
  // set the brightness of pin 9:, 0-31, 5 bit steps of brightness
//  analogWrite16(9, val[brightness]);
  analogWrite16(9, factor);
  
//  Serial.println(val[brightness]);
  
  // change the brightness for next time through the loop:
//  brightness = brightness + fadeAmount;
  
  // reverse the direction of the fading at the ends of the fade:
  
//  if (brightness == 0 || brightness == 31) {
//    fadeAmount = -fadeAmount ;
//  }
  
//  delay(FADESPEED); 

  delay(50);
  

}
