#ifndef _GATE_NAND_HPP
#define _GATE_NAND_HPP

#include "systemc.h"

template<unsigned int width>
class NAND : sc_module
{
public:
  sc_in<sc_uint<width> > Input;
  sc_out<bool> Output;

  SC_CTOR(NAND) :
    Input("Input"),
    Output("Output")
  {
    SC_METHOD(tick);
    sensitive << Input;
  }

  void tick() {
    Output.write( Input.read() != -1 );
  }
};

#endif
