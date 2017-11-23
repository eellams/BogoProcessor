#ifndef _REGISTER_NORMAL_HPP
#define _REGISTER_NORMAL_HPP

#include "systemc.h"

template<unsigned int width>
class register_normal : sc_module
{
public:
  sc_in<sc_uint<width> > Input;
  sc_out<sc_uint<width> > Output;
  sc_in<bool> Clock;
  sc_in<bool> Enable;
  sc_in<bool> N_Reset;

  SC_CTOR(register_normal) :
    Input("Input"),
    Output("Output"),
    Clock("Clock"),
    Enable("Enable"),
    N_Reset("N_Reset"),
    mLastClock()
  {
    SC_METHOD(tick);
    sensitive << Clock.pos();
    sensitive << N_Reset;
  }

  void tick() {
    if (N_Reset.read() == 0) {
      Output.write(0);
    }
    else if (Enable.read() == 1) {
      mLastClock = sc_time_stamp();
      std::clog << mLastClock << std::endl;
      Output.write(Input.read());
    }
  }

private:
  sc_time mLastClock;
};

#endif
