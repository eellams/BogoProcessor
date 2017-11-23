#include "systemc.h"

#include "gate_inv.hpp"
#include "register_normal.hpp"

#define WIDTH 8

int sc_main(int argc, char* argv[]) {
  sc_signal<sc_uint<WIDTH> > input;
  sc_signal<sc_uint<WIDTH> > inverted;
  sc_signal<sc_uint<WIDTH> > output;

  sc_signal<bool> reg_nreset;
  sc_signal<bool> reg_enable;

  sc_clock clk("Clock", 10, SC_NS, 0.5);
  INV<WIDTH> INV("Test_INV");
  register_normal<WIDTH> _register("Register");

  sc_trace_file *fp;

  INV.Input(input);
  INV.Output(inverted);

  _register.Input(inverted);
  _register.Output(output);
  _register.N_Reset(reg_nreset);
  _register.Enable(reg_enable);
  _register.Clock(clk);

  reg_nreset = true;
  reg_enable = true;

  fp = sc_create_vcd_trace_file("wave");
  fp->set_time_unit(100, SC_PS);
  sc_trace(fp, input, "Input");
  sc_trace(fp, inverted, "Inverted");
  sc_trace(fp, output, "Output");
  sc_trace(fp, reg_nreset, "N_Reset");
  sc_trace(fp, reg_enable, "Enable");
  sc_trace(fp, clk, "Clock");

  for (int i=0; i<(1<<WIDTH); i++) {
    input = i;
    sc_start(3, SC_NS);
  }

  sc_close_vcd_trace_file(fp);
  return 0;
}
