# hFramework

hFramework is a library for creating software for mechatronic devices (e.g. robots). It has the following ports:

- STM32 port for hardware created by Husarion - [CORE2 boards](https://husarion.com)
- Linux port for Raspberry Pi and Tinkerboard (experimental)
- ESP32 port (experimental)

## Using hFramework

hFramework documentation is available at http://docs.husarion.com. The easiest way to experience hFramework is to use [Husarion WebIDE](https://cloud.husarion.com) or install Husarion plugin to Visual Studio Code.

## Building hFramework

This section describes how to build hFramework yourself,

1. First install the dependencies. For example, on Ubuntu:

  ```
  sudo apt-get install cmake ninja-build gcc-arm-none-eabi
  ```

  On Windows, all required dependencies are bundled as part of VSCode plugin. You can also download standalone package: [HusarionTools-v3.exe](https://cdn.atomshare.net/cc70b0184feefaf7ead3741c58f98200cf8e017b/HusarionTools-v3.exe).

  A complete tutorial how to ude hFramework library can be found here: https://husarion.com/tutorials/other-tutorials/hframework-library-development/

2. Configure and build the project.

    ```
    mkdir build
    cd build && cmake -DBOARD_TYPE=core2 -GNinja ..
    ninja
    ```
    Substitute `core2` with `core2mini` if building for CORE2mini.
