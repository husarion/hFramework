# Low-level buses

RoboCORE supports I2C (Inter-Integrated Circuit, also called Two Wire Interface/TWI), SPI (Serial Peripheral Interface) and CAN (Controller Area Network).

## I2C

The I2C (Inter-Integrated Circuit) bus, pronounced I-squared-C is a multi-master bus used for connecting sensors and other devices to RoboCORE.

* `hExtI2C` -- an interface shared between both hExts
* `hSens1.i2c` and `hSens2.i2c`
* software I2C implementation on hSensors 3 and 4

--seeAlso hI2C

## SPI

The SPI (Serial Peripheral Interface) bus is a synchronous serial communication bus. It's used for connecting sensors, SD card and other device to RoboCORE. RoboCORE has two SPI interfaces:

* internal SD card SPI interface
* `hExtSPI` -- a SPI interface shared between both hExts

--seeAlso hSPI

{% if board(robocore) or board(core2) %}

## CAN

--example c++ can_send stripheaders:

--example c++ can_recv stripheaders:

The CAN bus (Controller Area Network) is a multi-master bus designed for communication between multiple microcontrollers. It has somewhat similar architecture to the original 10 Mbps Ethernet.

The CAN frame (CAN_frameRx, CAN_frameTx) contains the following fields:

 * `StdId` - first 11 bits of an address
 * `ExtId` - second 18 bits of an address
 * `RTR` - Remote transmission request - 0 for data frames, 1 for remote request frames
 * `IDE` - Identifier extension bit
 * `DLC` - Data length code - data length (0-8)
 * `data` - packet payload

--seeAlso hCAN
{% endif %}
