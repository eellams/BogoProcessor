#ifndef _GATE_NOR_HPP
#define _GATE_NOR_HPP

#include "systemc.h"

template<unsigned int width>
class NOR : sc_module
{
public:
  sc_in<sc_uint<width> > Input;
  sc_out<bool> Output;

  SC_CTOR(NOR) :
    Input("Input"),
    Output("Output")
  {
    SC_METHOD(tick);
    sensitive << Input;
  }

  void tick() {
    Output.write(Input.read() == 0);
  }
};

#endif
