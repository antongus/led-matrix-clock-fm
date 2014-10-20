led-matrix-clock-fm
===================

Source code of my [led matrix clock with fm radio](http://ahtoxa.net/micros/stm32/led-matrix-clock-fm/) project (RUS).
To clone project with submodules use
`git clone --recursive https://github.com/antongus/led-matrix-clock-fm.git`

####Prerequisites:
- GCC cross-compiler for ARM Cortex-M3 controllers (I use [gcc-arm-embedded](https://launchpad.net/gcc-arm-embedded));
- st-flash tool from [texane/stlink](https://github.com/texane/stlink) on linux, or
- ST-LINK_CLI utility from ST on Windows.

####Build and flash instructions:
- type `make` on shell prompt to build;
- `make program` to load program into microcontroller.

Instead of typing commands on shell prompt one can use Eclipse CDT IDE to build project and flash microcontroller.
Just load Eclipse project and select `build` and `program` from "make targets" window.
