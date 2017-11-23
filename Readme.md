BogoProcessor
=========

A silly simulation of a processor, where there is only one compulaory operation, that loads the contents of a source register into the destination register.

Every instruction is therefore composed of two equal width values, the source and address. Each operation takes one clock cycle.

| Upper half  | Lower half |
|-------------|------------|
| Destination |  Source    |

The current implementation has (note that these points could change during development):
* Automated registers (probably not the correct name)
  * For example, the MDR's value will change depending on the value of the MAR.
* General purpose registers
* Seperate instruction and data memory
  * Instruction width currently 8-bit. This allows 2 4-bit numbers as register addresses. There are therefore 16 addressable registers (see below)
  * Data width is 16-bit - but should be easily variable.
  * If memory was shared, instructions and data would be the same bit width. There would therefore be a compromise on width, as long width widths are prefereable for data (as longer numbers can be stored at any time), but this would mean more registers could be addressed - complicating the system.
* No pipelining
  * As each operation only takes one clock cycle to execute


The current implementation has the following addressable registers

| Address | Register | Full name                  | Brief Description                                       |
|---------|----------|----------------------------|---------------------------------------------------------|
| 0x00    | Z        | Zeo register               | Always contains 0x00                                    |
| 0x01    | PC       | Program Counter            | Stores the address of the next operation to be executed |
| 0x01    | MAR      | Memory Address Register    | Stores the address of data to be fetched from memory    |
| 0x02    | MDR      | Memory Data Register       | Stores the data at the address of MAR                   |
| 0x03    | TOS      | Top of stack               | Stores the data on the top of the stack                 |
| 0x04    | A        | ALU A                      | Stores one input to the ALU                             |
| 0x05    | B        | ALU B                      | Stores one input to the ALU                             |
| 0x06    | Q        | ALU Q                      | Stores the ALU output. Q := A + B                       |
| 0x07    |          |                            | To be confirmed                                         |
| 0x08    |          |                            | To be confirmed                                         |
| 0x09    |          |                            | To be confirmed                                         |
| 0x0A    | REG0     | General Purpose Register 0 | -                                                       |
| 0x0B    | REG1     | General Purpose Register 1 | -                                                       |
| 0x0C    | REG2     | General Purpose Register 2 | -                                                       |
| 0x0D    | REG3     | General Purpose Register 3 | -                                                       |
| 0x0E    | REG4     | General Purpose Register 4 | -                                                       |
| 0x0F    | REG5     | General Purpose Register 5 | -                                                       |

A breif overview of these registers can be found below:

### Z
Constant value of 0x00. Read/write protected.

### PC
Stores the address (in instruction memory) of the next operation to perform. Only the least significant bits actually matter (as instruction memory smaller than data memory). Automatically increases by 1 each clock cycle. Read and write

### MAR
Stores the memory address to be fetched and stored in MDR. When written to and changed, will causes the value in MDR to be updataed. Read and write.

### TOS
Stores the value at the top of the stack. When written to, pushes the old value onto the stack. Similarly, when read from, the value is updataed to become the item one below. Read and write.

### A
One of the inputs to the ALU. Write only (to prevent use as storage register).

### B
Another input to the ALU. Write only.

### Q
The asynchronous output of the ALU, such that Q := A + B. Read only.

### REG0...6
General purpose registers. Nothing particularly special about them. Read and write.
