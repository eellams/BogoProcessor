#ifndef _GATE_AND_HPP
#define _GATE_AND_HPP

#include "systemc.h"

template<unsigned int width>
class AND : sc_module
{
public:
  sc_in<sc_uint<width> > Input;
  sc_out<bool> Output;

  SC_CTOR(AND) :
    Input("Input"),
    Output("Output")
  {
    SC_METHOD(tick);
    sensitive << Input;
  }

  void tick() {
    Output.write(Input.read() == -1);
  }
};

#endif
