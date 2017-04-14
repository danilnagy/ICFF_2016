//sensor pins
#define trigPin_1 2
#define echoPin_1 3
#define trigPin_2 4
#define echoPin_2 5
#define trigPin_3 6
#define echoPin_3 7

//extra PWM pin
#define pin1 9
#define pin2 10

//sensor status pins
#define led_0 11
#define led_1 12
#define led_2 13

#include <Wire.h>
#include <NewPing.h>
#include <Adafruit_PWMServoDriver.h>
#include <avr/pgmspace.h>
#include <SoftwareSerial.h>

#define FADESPEED 1

#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// permutation array
PROGMEM const unsigned char p[] = {
  151, 160, 137, 91, 90, 15,
  131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
  190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
  88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
  77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
  102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
  135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
  5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
  223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
  129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
  251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
  49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
  138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
};

// array below generated with this code line in the INoise code.
// static { for (int i=0; i < 256 ; i++) fade[i] = (int)((1<<12)*f(i/256.)); }
PROGMEM const uint16_t fadeArray[] = {
 1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    2,    3,    3,    4,    6,    7,
  9,    10,   12,   14,   17,   19,   22,   25,   29,   32,   36,   40,   45,   49,   54,   60,
  65,   71,   77,   84,   91,   98,   105,  113,  121,  130,  139,  148,  158,  167,  178,  188,
  199,  211,  222,  234,  247,  259,  273,  286,  300,  314,  329,  344,  359,  374,  390,  407,
  424,  441,  458,  476,  494,  512,  531,  550,  570,  589,  609,  630,  651,  672,  693,  715,
  737,  759,  782,  805,  828,  851,  875,  899,  923,  948,  973,  998,  1023, 1049, 1074, 1100,
  1127, 1153, 1180, 1207, 1234, 1261, 1289, 1316, 1344, 1372, 1400, 1429, 1457, 1486, 1515, 1543,
  1572, 1602, 1631, 1660, 1690, 1719, 1749, 1778, 1808, 1838, 1868, 1898, 1928, 1958, 1988, 2018,
  2048, 2077, 2107, 2137, 2167, 2197, 2227, 2257, 2287, 2317, 2346, 2376, 2405, 2435, 2464, 2493,
  2523, 2552, 2580, 2609, 2638, 2666, 2695, 2723, 2751, 2779, 2806, 2834, 2861, 2888, 2915, 2942,
  2968, 2995, 3021, 3046, 3072, 3097, 3122, 3147, 3172, 3196, 3220, 3244, 3267, 3290, 3313, 3336,
  3358, 3380, 3402, 3423, 3444, 3465, 3486, 3506, 3525, 3545, 3564, 3583, 3601, 3619, 3637, 3654,
  3672, 3688, 3705, 3721, 3736, 3751, 3766, 3781, 3795, 3809, 3822, 3836, 3848, 3861, 3873, 3884,
  3896, 3907, 3917, 3928, 3937, 3947, 3956, 3965, 3974, 3982, 3990, 3997, 4000, 4000, 4000, 4000,
  4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000,
  4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000,
};

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

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

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

long perlinTimeInc = 2500L;
long perlinXInc    = 100L;
long perlinYInc    = 100L;
float targetValue = 3500; // variable to control overall light level

// returns a value between 0 - 255 for lights
int renderNoise(unsigned long x, unsigned long y, unsigned long z)
{
  return 137 + (inoise(x, y, z) >> 9);
}

#define P(x) pgm_read_byte_near(p + ((x)&255))

long inoise(unsigned long x, unsigned long y, unsigned long z)
{
  long X = x >> 16 & 255, Y = y >> 16 & 255, Z = z >> 16 & 255, N = 1L << 16;
  x &= N - 1; y &= N - 1; z &= N - 1;

  long u = fade(x), v = fade(y), w = fade(z), A = P(X) + Y, AA = P(A) + Z, AB = P(A + 1) + Z, B = P(X + 1) + Y, BA = P(B) + Z, BB = P(B + 1) + Z;

  return lerp(w, lerp(v, lerp(u, grad(P(AA), x   , y   , z   ),
                              grad(P(BA), x - N , y   , z   )),
                      lerp(u, grad(P(AB), x   , y - N , z   ),
                           grad(P(BB), x - N , y - N , z   ))),
              lerp(v, lerp(u, grad(P(AA + 1), x   , y   , z - N ),
                           grad(P(BA + 1), x - N , y   , z - N )),
                   lerp(u, grad(P(AB + 1), x   , y - N , z - N ),
                        grad(P(BB + 1), x - N , y - N , z - N ))));
}

long lerp(long t, long a, long b) {
  return a + (t * (b - a) >> 12);
}

long grad(long hash, long x, long y, long z)
{ long h = hash & 15, u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : z;
  return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

#define F(x) pgm_read_word_near(fadeArray + (x))

long fade(long t)
{
  long t0 = F(t >> 8), t1 = F(min(255, (t >> 8) + 1));
  return t0 + ( (t & 255) * (t1 - t0) >> 8 );
}


class Sensor {

  public:
    int x, y, id;
    boolean status;

    Sensor(int _id, int _x, int _y, boolean _s) {
      id = _id;
      x = _x;
      y = _y;
      status = _s;
    }

    void updateStatus(boolean _status) {
      status = _status;
    }

    void toggleStatus() {
      status = !status;
    }

    float getRange(int _x, int _y) {
      if (status) {
        float dx = x - _x;
//        float dy = y - _y;
        float dy = 0;
        
        //calculate distance
        float d = sqrt( pow(dx, 2) + pow(dy, 2) );
        //convert distance to go from negative to positive
        float t = (-d + 150) / 5;
        //implement siqmoid function
        float sig = 1.0 / (1.0 + exp(-t));
        
        return (sig * 5);
      } else {
        return 0;
      }
    }

};

class Light {

  public:
    float x, y;
    int id, level, adjustment;

    Light(int _id, float _x, float _y)
    {
      x = _x;
      y = _y;
      id = _id;
      level = 0;
      adjustment = 0;
    };

    void update(int _c, Sensor &sensor1, Sensor &sensor2, Sensor &sensor3) {

      if (adjustment < 300) {
        adjustment += sensor1.getRange(x, y);
        adjustment += sensor2.getRange(x, y);
        adjustment += sensor3.getRange(x, y);
      }

      if (adjustment > 0) {
        adjustment -= 2;
      }

      level = int(renderNoise(x * perlinXInc, y * perlinYInc, _c * perlinTimeInc)) + 2 * adjustment;

    };

    String plot() {
//      if (level > 127) return "*";
//      else             return "-";
      return String(this -> getValueNorm());
    };

    int getValue() {
      return level;
    }

    int getValueNorm() {
      return constrain(level, 5, 250);
    }

    int getValueFixed() {
      if (level < 50){
        return 0;
      } else if (level < 100) {
        return 10;
      } else if (level < 150) {
        return 330;
      } else if (level < 200) {
        return 4000;
      }
    }

    void adjustValue(float _multiplier) {
      level *= _multiplier;
    }
};

Light *lights[17];
Sensor *sensors[3];

int count = 0;

NewPing sensor_1(trigPin_1, echoPin_1, MAX_DISTANCE); 
NewPing sensor_2(trigPin_2, echoPin_2, MAX_DISTANCE); 
NewPing sensor_3(trigPin_3, echoPin_3, MAX_DISTANCE); 

#define pin1 9
#define pin2 10

SoftwareSerial mySerial(5, 6); // RX, TX

int brightness = 0;    // initial brightness of LED
int fadeAmount = 1;

void setup()
{
  #ifdef ESP8266
    Wire.pins(2, 14);   // ESP8266 can use any two pins, such as SDA to #2 and SCL to #14
  #endif

//  Serial.begin(115200);

  pinMode(trigPin_1, OUTPUT);
  pinMode(echoPin_1, INPUT);
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
  pinMode(trigPin_3, OUTPUT);
  pinMode(echoPin_3, INPUT);

  pinMode(led_0, OUTPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);

  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  setupPWM16();

  pwm.begin();
  pwm.setPWMFreq(1000);  // This is the maximum PWM frequency
/*
  lights[0] = new Light(0, 0, 0);
  lights[1] = new Light(1, 180, 60);
  lights[2] = new Light(2, 300, 60);
  lights[3] = new Light(3, 420, 60);
  lights[4] = new Light(4, 540, 60);
  lights[5] = new Light(5, 660, 60);
  lights[6] = new Light(6, 120, 160);
  lights[7] = new Light(7, 240, 160);
  lights[8] = new Light(8, 360, 160);
  lights[9] = new Light(9, 480, 160);
  lights[10] = new Light(10, 600, 160);
  lights[11] = new Light(11, 60, 260);
  lights[12] = new Light(12, 180, 260);
  lights[13] = new Light(13, 300, 260);
  lights[14] = new Light(14, 420, 260);
  lights[15] = new Light(15, 540, 260);
  lights[16] = new Light(16, 660, 260);
*/
  lights[0] = new Light(0, 60, 260);
  lights[1] = new Light(1, 0, 0);
  lights[2] = new Light(2, 180, 60);
  lights[3] = new Light(3, 240, 160);
  lights[4] = new Light(4, 540, 260);
  lights[5] = new Light(5, 660, 260);
  lights[6] = new Light(6, 420, 260);
  lights[7] = new Light(7, 180, 260);
  lights[8] = new Light(8, 300, 260);
  lights[9] = new Light(9, 480, 160);
  lights[10] = new Light(10, 600, 160);
  lights[11] = new Light(11, 420, 60);
  lights[12] = new Light(12, 540, 60);
  lights[13] = new Light(13, 120, 160);
  lights[14] = new Light(14, 360, 160);
  lights[15] = new Light(15, 300, 60);
  lights[16] = new Light(16, 660, 60);

  sensors[0] = new Sensor(16, 120, 227, false);
  sensors[1] = new Sensor(16, 360, 227, false);
  sensors[2] = new Sensor(16, 600, 227, false);
  
  TWBR = 12;
  
  mySerial.begin(4800);
  mySerial.listen();
  
  Serial.begin(57600);
}

void loop()
{
//  delay(1000);
//  
//  //READ SENSOR VALUES
//  float distance;
//  
//  distance = sensor_1.ping_cm();
//  if (distance > 0 && distance < 50) {
//    sensors[0] -> updateStatus(true);
//    digitalWrite(led_0, HIGH);
//    Serial.println(distance);
//  } else {
//    sensors[0] -> updateStatus(false);
//    digitalWrite(led_0, LOW);
//  }
//  
//  distance = sensor_2.ping_cm();
//  if (distance > 0 && distance < 50) {
//    sensors[1] -> updateStatus(true);
//    digitalWrite(led_1, HIGH);
//  } else {
//    sensors[1] -> updateStatus(false);
//    digitalWrite(led_1, LOW);
//  }
//  
//  distance = sensor_3.ping_cm();
//  if (distance > 0 && distance < 50) {
//    sensors[2] -> updateStatus(true);
//    digitalWrite(led_2, HIGH);
//  } else {
//    sensors[2] -> updateStatus(false);
//    digitalWrite(led_2, LOW);
//  }

  //UPDATE LIGHT VALUES
  for (int i = 0; i < 17; i++) {
    lights[i] -> update(count, *sensors[0], *sensors[1], *sensors[2]);
  }

  //SUM CURRENT VALUES
  int values = 0;
  for (int i = 0; i < 17; i++) {
    values += lights[i] -> getValue();
  }

  //ADJUST VALUES TO HIT TARGET
  float multiplier = targetValue / values;
  for (int i = 0; i < 17; i++) {
    lights[i] -> adjustValue(multiplier);
  }

  //CHECK TARGET
//  values = 0;
//  for (int i = 0; i < 17; i++) {
//    values += lights[i] -> getValue();
//  }
//  Serial.println(values);

  
  
  //WRITE LIGHT VALUES TO PWM
  for (uint8_t pwmnum = 0; pwmnum < 8; pwmnum++) {
      pwm.setPWM(pwmnum, 0, val(int(lights[pwmnum] -> getValueNorm())) );
  }
  
  analogWrite16(pin1, val(int(lights[9] -> getValueNorm())));
  analogWrite16(pin2, val(int(lights[10] -> getValueNorm())));
  
  mySerial.write((byte)0);
  mySerial.write((byte)(int(lights[11] -> getValueNorm())));
  mySerial.write((byte)(int(lights[12] -> getValueNorm())));
  Serial.println(int(lights[12] -> getValueNorm()));
  delay(10);
  
  mySerial.write((byte)1);
  mySerial.write((byte)(int(lights[13] -> getValueNorm())));
  mySerial.write((byte)(int(lights[14] -> getValueNorm())));
  delay(10);
  
  mySerial.write((byte)2);
  mySerial.write((byte)(int(lights[15] -> getValueNorm())));
  mySerial.write((byte)(int(lights[16] -> getValueNorm())));
  Serial.println(int(lights[16] -> getValueNorm()));
  delay(10);


/*
  //FADE TEST SEQUENCE
  pwm.setPWM(0, 0, val(250) );
  pwm.setPWM(1, 0, val(250) );
  pwm.setPWM(2, 0, val(250) );
  pwm.setPWM(3, 0, val(250) );
  pwm.setPWM(4, 0, val(250) );
  pwm.setPWM(5, 0, val(250) );
  pwm.setPWM(6, 0, val(250) );
  pwm.setPWM(7, 0, val(250) );
  pwm.setPWM(8, 0, val(250) );
  
  analogWrite16(pin1, val(250));
  analogWrite16(pin2, val(250));
  
  mySerial.write((byte)0);
  mySerial.write((byte)(250)); //
  mySerial.write((byte)(250)); //
  delay(10);
  
  mySerial.write((byte)1);
  mySerial.write((byte)(250)); //
  mySerial.write((byte)(250));
  delay(10);
  
  mySerial.write((byte)2);
  mySerial.write((byte)(250));
  mySerial.write((byte)(250));
  delay(10);
*/

  //SERIAL VISUALIZATION
//  Serial.print(lights[0] -> plot());
//  Serial.print(" ");
//  Serial.print(lights[1] -> plot());
//  Serial.print(" ");
//  Serial.print(lights[2] -> plot());
//  Serial.print(" ");
//  Serial.print(lights[3] -> plot());
//  Serial.print(" ");
//  Serial.print(lights[4] -> plot());
//  Serial.print(" ");
//  Serial.print(lights[5] -> plot());
//  Serial.print(" ");
//  Serial.print(lights[6] -> plot());
//  Serial.print(" ");
//  Serial.print(lights[7] -> plot());
//  Serial.print(" ");
//  Serial.print(lights[8] -> plot());
//  Serial.print(" ");
//  Serial.print(lights[9] -> plot());
//  Serial.print(" ");
//  Serial.print(lights[10] -> plot());
//  Serial.print(" ");
//  Serial.print(lights[11] -> plot());
//  Serial.print(" ");
//  Serial.print(lights[12] -> plot());
//  Serial.print(" ");
//  Serial.print(lights[13] -> plot());
//  Serial.print(" ");
//  Serial.print(lights[14] -> plot());
//  Serial.print(" ");
//  Serial.print(lights[15] -> plot());
//  Serial.print(" ");
//  Serial.println(lights[16] -> plot());

  brightness = brightness + fadeAmount;
  
  if (brightness < 0 || brightness > 255) {
    fadeAmount = -fadeAmount ;
  }

//  delay(FADESPEED); 

  count++;
  if (count > 10000){
    count = 0;
  }
}
