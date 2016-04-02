#define PIN1 9
#define PIN2 10

#define FADESPEED 100

//#define __PROG_TYPES_COMPAT__
//#include <avr/pgmspace.h>
//#define CIELPWM(a) (pgm_read_word_near(CIEL8 + a)) // CIE Lightness loopup table

int val[] = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,
  8,8,8,8,9,9,9,9,10,10,10,11,11,12,12,12,13,13,14,14,15,15,15,16,17,17,18,18,19,19,20,21,21,22,23,24,24,25,26,27,28,29,30,31,
  32,33,34,35,36,37,39,40,41,43,44,45,47,49,50,52,53,55,57,59,61,63,65,67,69,72,74,77,79,82,84,87,90,93,96,99,103,106,110,113,
  117,121,125,129,133,138,142,147,152,157,162,168,173,179,185,191,197,204,210,217,225,232,240,248,256,264,273,282,292,301,311,
  322,332,343,355,366,379,391,404,418,431,446,460,476,492,508,525,542,560,579,598,618,638,659,681,704,727,751,776,802,828,856,
  884,914,944,975,1007,1041,1075,1111,1148,1186,1225,1266,1308,1351,1396,1442,1490,1539,1591,1643,1698,1754,1812,1872,1934,1999,
  2065,2133,2204,2277,2353,2431,2511,2594,2680,2769,2861,2956,3054,3155,3260,3368,3480,3595,3714,3837,3965,4096
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
  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  setupPWM16();
}

void loop() {
  
  
  // set the brightness of pin 9:, 0-31, 5 bit steps of brightness
  analogWrite16(9, val[brightness+0]);
  analogWrite16(10, val[255-brightness]);
  
  Serial.println(val[brightness]);
  
  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;
  
  // reverse the direction of the fading at the ends of the fade:
  
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ;
  }
    // wait for 500 milliseconds to see the bightness change
  delay(FADESPEED); 
  
//  analogWrite16(9, 15);
//  delay(FADESPEED);
//  analogWrite16(9, 4095);
//  delay(FADESPEED);

//  static uint16_t i;
  
//  for (i = 1; i < 4095; i++) { 
//    analogWrite16(PIN, i);
//    Serial.println(i);
//    delay(FADESPEED);
//  } 
//  for (i = 4095; i > 1; i--) { 
//    analogWrite16(PIN, i);
//    Serial.println(i);
//    delay(FADESPEED);
//  } 

    
//    analogWrite16(9, i);
//    analogWrite16(10, 0xffff - i);
//    i++;
//    delay(1);
    
    

}
