SRV-1 CE is an alternative firmware for the SRV-1 robot (www.surveyor.com). Currently the project is in it's infancy, however hopefully all functionality will be replicated. The code is structured differently to the official firmware, using interrupts and counters to allow a crude form of multitasking. Currently only the very basics are in place (and code is fluctuating rapidly), however as time passes the project should settle down.

**Here is the current list of 'features':**
  * Tight, fully modular code structure. Each part of the functionality is self contained, allowing parts to added/removed easily. Modules try to be as generic as possible, allowing code reuse.
  * Fully interrupt driven UART
  * No startup assembly (project uses default crt0.s supplied by GCC. PLL and SDRAM setup in C).
  * No hard coded clock speed. The core and peripheral clock can be controlled by a single  #define. All peripheral timing requirements are calculated automatically based on the defines.
  * Newlib C library, with working printf() statements.

**Currently supported peripherals/features:**
  * UART communication (with one-way CTS/RTS).
  * SDRAM (with working malloc()).
  * LEDs.
  * Core Timer (with 16 (adjustable) software counters).
  * i2c communication (Master only, optional Omnivision SCCB mode).
  * Lasers (individual left/right on/off/toggle control).
  * Motor control (left/right percentages).