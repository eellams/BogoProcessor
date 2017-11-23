#ifndef _GATE_INV_HPP
#define _GATE_INV_HPP

#include "systemc.h"

template<unsigned int width>
class INV : sc_module
{
public:
  sc_in<sc_uint<width> > Input;
  sc_out<sc_uint<width> > Output;

  SC_CTOR(INV) :
    Input("Input"),
    Output("Output")
  {
    SC_METHOD(tick);
    sensitive << Input;
  }

  void tick() {
    Output.write(~Input.read());
  }
};

#endif
