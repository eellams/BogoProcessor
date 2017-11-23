#ifndef _GATE_OR_HPP
#define _GATE_OR_HPP

#include "systemc.h"

template<unsigned int width>
class OR : sc_module
{
public:
  sc_in<sc_uint<width> > Input;
  sc_out<bool> Output;

  SC_CTOR(OR) :
    Input("Input"),
    Output("Output")
  {
    SC_METHOD(tick);
    sensitive << Input;
  }

  void tick() {
    Output.write(Input.read() != 0);
  }
};

#endif
