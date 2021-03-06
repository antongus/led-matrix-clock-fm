led-matrix-clock-fm
===================

![led matrix clock with FM radio][picture]

Source code of my [led matrix clock with FM radio][1] project (RUS).
To clone project with submodules use
`git clone --recursive https://github.com/antongus/led-matrix-clock-fm.git`

####Prerequisites:
- GCC cross-compiler for ARM Cortex-M3 controllers (I use [gcc-arm-embedded][2]);
- st-flash tool from [texane/stlink][3] on linux, or
- ST-LINK_CLI utility from ST on Windows.

####Build and flash instructions:
- type `make` on shell prompt to build;
- `make program` to load program into microcontroller.

Instead of typing commands on shell prompt one can use Eclipse CDT IDE to build project and flash microcontroller.
Just load Eclipse project and select `build` and `program` from "make targets" window.

[1]: http://ahtoxa.net/micros/stm32/led-matrix-clock-fm/
[2]: https://launchpad.net/gcc-arm-embedded
[3]: https://github.com/texane/stlink
[picture]: https://cloud.githubusercontent.com/assets/6680984/10188008/9fc17808-6775-11e5-832f-634c178aa638.jpg
