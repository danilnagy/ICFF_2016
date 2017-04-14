
#include <SoftwareSerial.h>

PROGMEM const uint16_t lightMap[] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 
  3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7,
  8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 11, 11, 12, 12, 12, 13, 13, 14, 14, 15, 15, 15, 16, 17, 
  17, 18, 18, 19, 19, 20, 21, 21, 22, 23, 24, 24, 25, 26, 27, 28, 29, 30, 31,
  32, 33, 34, 35, 36, 37, 39, 40, 41, 43, 44, 45, 47, 49, 50, 52, 53, 55, 57, 59, 61, 63, 65, 
  67, 69, 72, 74, 77, 79, 82, 84, 87, 90, 93, 96, 99, 103, 106, 110, 113,
  117, 121, 125, 129, 133, 138, 142, 147, 152, 157, 162, 168, 173, 179, 185, 191, 197, 204, 
  210, 217, 225, 232, 240, 248, 256, 264, 273, 282, 292, 301, 311,
  322, 332, 343, 355, 366, 379, 391, 404, 418, 431, 446, 460, 476, 492, 508, 525, 542, 560, 
  579, 598, 618, 638, 659, 681, 704, 727, 751, 776, 802, 828, 856,
  884, 914, 944, 975, 1007, 1041, 1075, 1111, 1148, 1186, 1225, 1266, 1308, 1351, 1396, 1442, 
  1490, 1539, 1591, 1643, 1698, 1754, 1812, 1872, 1934, 1999,
  2065, 2133, 2204, 2277, 2353, 2431, 2511, 2594, 2680, 2769, 2861, 2956, 3054, 3155, 3260, 
  3368, 3480, 3595, 3714, 3837, 3965, 4000
};

int val(int x) {
  return pgm_read_word_near(lightMap + x);
}

#define pin1 9
#define pin2 10

int id = 2;

SoftwareSerial mySerial(5, 6); // RX, TX


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

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  
  pinMode(13, OUTPUT);

  // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);
  mySerial.listen();
  
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  
  setupPWM16();
}

int offset = 0;

void loop() // run over and over
{
    if (mySerial.available() == (3)) {
      
      Serial.println("transmission received");
      
      int chan = mySerial.read();
      int in1 = mySerial.read();
      int in2 = mySerial.read();
      
      if (chan == id){
//        Serial.println(chan);
//        Serial.println(val(in1));
        analogWrite16(pin1, val(in1));
//        Serial.println(val(in2));
        analogWrite16(pin2, val(in2));
      }
      
      if (chan > 2){
//        Serial.print("received channel byte: ");
//        Serial.println(chan);
//        Serial.println("offsetting");
        //dump extra byte
        mySerial.read();
      }
    }
}

