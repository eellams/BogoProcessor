#ifndef _GATE_NOT_HPP
#define _GATE_NOT_HPP

#include "systemc.h"

class NOT : sc_module
{
public:
  sc_in<bool> Input;
  sc_out<bool> Output;

  SC_CTOR(NOT) :
    Input("Input"),
    Output("Output")
  {
    SC_METHOD(tick);
    sensitive << Input;
  }

  void tick() {
    Output.write(!Input.read());
  }
};

#endif
