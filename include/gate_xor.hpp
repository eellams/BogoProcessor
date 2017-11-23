#ifndef _GATE_XOR_HPP
#define _GATE_XOR_HPP

#include "systemc.h"
#include "helpers.hpp"

// Q is 1 if one and only one of the input bits is 1.
template<unsigned int width>
class XOR : sc_module
{
public:
  sc_in<sc_uint<width> > Input;
  sc_out<bool> Output;

  SC_CTOR(XOR) :
    Input("Input"),
    Output("Output")
  {
    SC_METHOD(tick);
    sensitive << Input;
  }

  void tick() {
    sc_uint<width> read = Input.read();
    int numberOfBits = numberOfSetBits<width>(read);

    Output.write(numberOfBits == 1);
  }
};

#endif
