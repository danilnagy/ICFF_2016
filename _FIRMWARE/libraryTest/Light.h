/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Light_h
#define Light_h

#include "Arduino.h"

class Light
{
  public:
    Light(int _id, int _x, int _y);
    int getLocation();
  private:
//    Vec location = new Vec(0,0);
    float x, y;
    int id, level, adjustment;
//    int _pin;
};

#endif
