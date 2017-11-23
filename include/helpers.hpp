#ifndef _HELPERS_HPP
#define _HELPERS_HPP

#include "systemc.h"
#include <map>

std::map<unsigned char, int> Lookup;
bool Setup = false;

void setup() {
  for (unsigned int i=0; i <= 0xff; i++) {
    int numberof1s = 0;
    int temp = i;
    for (unsigned int j=0; j<8; j++) {
      numberof1s += temp & 0x01;
      temp = temp >> 1;
    }
    Lookup[i] = numberof1s;
  }

  Setup = true;
}


template <unsigned int width>
int numberOfSetBits(const sc_uint<width> &in) {
  if (!Setup) setup();

  int numberOfShifts = width / 8 + 1;
  int toReturn = 0;

  //std::clog << "A" << std::endl;
  sc_uint<width> tempIn = in;
  for (int i=0; i<numberOfShifts; i++) {
    unsigned char key = tempIn & 0xff;
    tempIn = tempIn >> 8;
    toReturn += Lookup[key];
  }
  return toReturn;
}

#endif
