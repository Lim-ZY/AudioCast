Connections from MSP430 to Si4713:

External Pullup Resistors required for I2C

MSP430 -> Si4713

Data: P1.2 -> LIN/RIN

SDA: P1.6 -> SDA

SCL: P1.7 -> SCL

RST: RST -> RST

VCC: VCC -> VIN

GND: GND -> GND

## Building

### Code Composer Studio (Windows)

1. Go to the taskbar and select File.
2. Select _Import Project(s)..._ and navigate to the MSP430 folder before selecting _Finish_.
3. Right click on the AudioCast folder and select properties.
4. Navigate to _MSP430 Compiler -> Processor Options_ and set:
    - Silicon version = mspx
    - Code memory model = small
    - Data memory model = small
5. Navigate to _MSP430 Compiler -> Optimization_ and set:
    - Optimization level = 4 - Whole Program Optimizations
    - Speed vs. size trade-offs = 0 (size)
6. Navigate to _MSP430 Compiler -> Advanced Options -> Language Options_ and set
    Level of printf/scanf support required = full
7. Navigate to _MSP430 Linker -> Basic Options_ and set:
    - Heap size for C/C++ dynamic memory allocation = 352
    - Set C system stack size = 240
8. Select run and click _Flash Project_.

### Code Composer Studio (Linux, macOS)

TBD

### PlatformIO 

TBD