#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <vector>
#include <cmath>

#include <boost/test/included/unit_test.hpp>

#include "register_normal.hpp"
#include "gate_nand.hpp"
#include "gate_and.hpp"
#include "gate_nor.hpp"
#include "gate_or.hpp"
#include "gate_xor.hpp"
#include "gate_inv.hpp"
#include "gate_not.hpp"

#define WIDTH 8
#define RANDOM false

register_normal<WIDTH> test_register("Test_Register");
sc_signal<sc_uint<WIDTH> > test_register_input;
sc_signal<sc_uint<WIDTH> > test_register_output;
sc_signal<bool> test_register_enable;
sc_signal<bool> test_register_clock;
sc_signal<bool> test_register_nreset;

NAND<WIDTH> test_nand("Test_NAND_gate");
sc_signal<sc_uint<WIDTH> > test_nand_input;
sc_signal<bool> test_nand_output;

AND<WIDTH> test_and("Test_AND_gate");
sc_signal<sc_uint<WIDTH> > test_and_input;
sc_signal<bool> test_and_output;

NOR<WIDTH> test_nor("Test_NOR_gate");
sc_signal<sc_uint<WIDTH> > test_nor_input;
sc_signal<bool> test_nor_output;

OR<WIDTH> test_or("Test_OR_gate");
sc_signal<sc_uint<WIDTH> > test_or_input;
sc_signal<bool> test_or_output;

XOR<WIDTH> test_xor("Test_XOR_gate");
sc_signal<sc_uint<WIDTH> > test_xor_input;
sc_signal<bool> test_xor_output;

INV<WIDTH> test_inverter("Test_INV_gate");
sc_signal<sc_uint<WIDTH> > test_inverter_input;
sc_signal<sc_uint<WIDTH> > test_inverter_output;

NOT test_not("Test_NOT_gate");
sc_signal<bool> test_not_input;
sc_signal<bool> test_not_output;

bool initialised = false;
void initialise_connections() {
  test_register.Input(test_register_input);
  test_register.Output(test_register_output);
  test_register.Enable(test_register_enable);
  test_register.Clock(test_register_clock);
  test_register.N_Reset(test_register_nreset);

  test_nand.Input(test_nand_input);
  test_nand.Output(test_nand_output);

  test_and.Input(test_and_input);
  test_and.Output(test_and_output);

  test_nor.Input(test_nor_input);
  test_nor.Output(test_nor_output);

  test_or.Input(test_or_input);
  test_or.Output(test_or_output);

  test_xor.Input(test_xor_input);
  test_xor.Output(test_xor_output);

  test_inverter.Input(test_inverter_input);
  test_inverter.Output(test_inverter_output);

  test_not.Input(test_not_input);
  test_not.Output(test_not_output);

  initialised = true;
}

bool PROTO_NOT(const bool &a) {
  return !a;
}

template<const unsigned int N>
bool PROTO_OR(const sc_uint<N> &a) {
  return a != 0;
}

template<const unsigned int N>
bool PROTO_AND(const sc_uint<N> &a) {
  return a == -1;
}

template<const unsigned int N>
bool PROTO_NOR(const sc_uint<N> &a) {
  return PROTO_NOT(PROTO_OR<N>(a));
}

template<const unsigned int N>
bool PROTO_NAND(const sc_uint<N> &a) {
  return PROTO_NOT(PROTO_AND<N>(a));
}

template<const unsigned int N>
bool PROTO_XOR(const sc_uint<N> &a) {
  int numberOfBits = 0;
  sc_uint<N> temp = a;
  for (unsigned int i=0; i<N; i++) {
    numberOfBits += temp & 0x01;
    temp = temp >> 1;
  }

  return numberOfBits == 1;
}

template<const unsigned int N>
sc_uint<N> PROTO_INV(const sc_uint<N> &a) {
  return (1 << N) - 1 - a;
}

template<const unsigned int N>
std::vector<sc_uint<N> > comb_generate_test_vector(const bool &shuffle = RANDOM) {
  const unsigned long long maximumNumber = 1 << N;
  std::vector<sc_uint<N> > toReturn;

  // So no time is wasted resizing the vector
  toReturn.reserve(maximumNumber);

  // Add all of the numbers
  for (unsigned long long i = 0; i<maximumNumber; i++) {
    toReturn.push_back(i);
  }

  // Shuffle if required
  if (shuffle)
    std::random_shuffle (toReturn.begin(), toReturn.end());

  return toReturn;
}

BOOST_AUTO_TEST_CASE( test_register_ )
{
  std::vector<sc_uint<WIDTH> > aVec;

  if(!initialised) initialise_connections();
  aVec = comb_generate_test_vector<WIDTH>();

  test_register_clock.write(false);
  sc_start(10,SC_NS);

  test_register_enable.write(true);
  test_register_nreset.write(true);
  test_register_clock.write(false);

  for(std::vector<sc_uint<WIDTH> >::iterator aIt = aVec.begin(); aIt != aVec.end(); ++aIt) {
    // Check the async reset
    test_register_input.write(-1);
    test_register_nreset = 0;
    sc_start(1, SC_NS);
    BOOST_CHECK(test_register_output.read() == 0);
    test_register_nreset = 1;
    sc_start(1,SC_NS);

    // Clock in data
    test_register_input.write(*aIt);
    sc_start(1, SC_NS);
    test_register_clock.write(true);
    sc_start(1, SC_NS);
    test_register_clock.write(false);
    sc_start(1, SC_NS);
    BOOST_CHECK(test_register_output.read() == *aIt);

    // Check enable
    test_register_input.write(0);
    test_register_enable = 0;
    test_register_input.write(*aIt);
    sc_start(1, SC_NS);
    test_register_clock.write(true);
    sc_start(1, SC_NS);
    test_register_enable = 1;
    test_register_clock.write(false);
    sc_start(1, SC_NS);
    BOOST_CHECK(test_register_output.read() == *aIt);

    // TODO check enable and reset at the same time?
  }
}

BOOST_AUTO_TEST_CASE( test_nand_gate )
{
  std::vector<sc_uint<WIDTH> > aVec;

  if (!initialised) initialise_connections();

  aVec = comb_generate_test_vector<WIDTH>();

  sc_start(10, SC_NS);

  for(std::vector<sc_uint<WIDTH> >::iterator aIt = aVec.begin(); aIt != aVec.end(); ++aIt) {
    test_nand_input.write(*aIt);
    sc_start(1, SC_NS);
    BOOST_CHECK(test_nand_output.read() == PROTO_NAND<WIDTH>(*aIt));
  }
  sc_pause();
}

BOOST_AUTO_TEST_CASE( test_and_gate )
{
  std::vector<sc_uint<WIDTH> > aVec;

  if (!initialised) initialise_connections();

  aVec = comb_generate_test_vector<WIDTH>();

  sc_start(10, SC_NS);

  for(std::vector<sc_uint<WIDTH> >::iterator aIt = aVec.begin(); aIt != aVec.end(); ++aIt) {
    test_and_input.write(*aIt);
    sc_start(1, SC_NS);
    BOOST_CHECK(test_and_output.read() == PROTO_AND<WIDTH>(*aIt));
  }
  sc_pause();
}

BOOST_AUTO_TEST_CASE( test_nor_gate )
{
  std::vector<sc_uint<WIDTH> > aVec;

  if (!initialised) initialise_connections();

  aVec = comb_generate_test_vector<WIDTH>();
  sc_start(10, SC_NS);

  for(std::vector<sc_uint<WIDTH> >::iterator aIt = aVec.begin(); aIt != aVec.end(); ++aIt) {
    test_nor_input.write(*aIt);
    sc_start(1, SC_NS);
    BOOST_CHECK(test_nor_output.read() == PROTO_NOR<WIDTH>(*aIt));
  }
  sc_pause();
}

BOOST_AUTO_TEST_CASE( test_or_gate )
{
  std::vector<sc_uint<WIDTH> > aVec;

  if (!initialised) initialise_connections();

  aVec = comb_generate_test_vector<WIDTH>();

  sc_start(10, SC_NS);

  for(std::vector<sc_uint<WIDTH> >::iterator aIt = aVec.begin(); aIt != aVec.end(); ++aIt) {
    test_or_input.write(*aIt);

    sc_start(1, SC_NS);
    BOOST_CHECK(test_or_output.read() == PROTO_OR<WIDTH>(*aIt));
  }
  sc_pause();
}

BOOST_AUTO_TEST_CASE( test_xor_gate )
{
  std::vector<sc_uint<WIDTH> > aVec;

  if (!initialised) initialise_connections();

  aVec = comb_generate_test_vector<WIDTH>();

  sc_start(10, SC_NS);

  for(std::vector<sc_uint<WIDTH> >::iterator aIt = aVec.begin(); aIt != aVec.end(); ++aIt) {
    test_xor_input.write(*aIt);

    sc_start(1, SC_NS);
    BOOST_CHECK(test_xor_output.read() == PROTO_XOR<WIDTH>(*aIt));
  }
  sc_pause();
}

BOOST_AUTO_TEST_CASE( test_inv_gate )
{
  std::vector<sc_uint<WIDTH> > aVec;

  if (!initialised) initialise_connections();

  aVec = comb_generate_test_vector<WIDTH>();

  sc_start(10, SC_NS);

  for(std::vector<sc_uint<WIDTH> >::iterator aIt = aVec.begin(); aIt != aVec.end(); ++aIt) {
    test_inverter_input.write(*aIt);

    sc_start(1, SC_NS);
    BOOST_CHECK(test_inverter_output.read() == PROTO_INV<WIDTH>(*aIt));
  }

  sc_pause();
}

BOOST_AUTO_TEST_CASE( test_not_gate )
{
  std::vector<bool> aVec;

  if (!initialised) initialise_connections();

  aVec.push_back(true);
  aVec.push_back(false);

  sc_start(10, SC_NS);

  for(std::vector<bool>::iterator aIt = aVec.begin(); aIt != aVec.end(); ++aIt) {
    test_not_input.write(*aIt);

    sc_start(1, SC_NS);
    BOOST_CHECK(test_not_output.read() == PROTO_NOT(*aIt));
  }

  sc_pause();
}

int sc_main(int argc, char* argv[]) {
  return 0;
}
