# Serial

--example c++ serial.cpp func:hMain

Serial is used for communication between RoboCORE and other devices.

{% if board(robocore) %}
RoboCORE has several serial interfaces:

* `Serial` -- the [USB-serial](https://wiki.robocore.io/hardware:usb_device) interface
* `hExt1.serial` -- the serial interface on [hExt1](https://wiki.robocore.io/hardware:hext)
* `hExt2.serial` -- the serial interface on [hExt2](https://wiki.robocore.io/hardware:hext)
* `Edison` -- the serial interface to [Intel Edison](https://wiki.robocore.io/edison:start). You shouldn't directly use it in normal configurations.
* hSensors 3 (see [the chapter about hSensors](#hsensors))
{% elif board(core2) %}
Husarion CORE2 has several serial interfaces:

* `Serial` -- the [USB-serial](https://wiki.robocore.io/hardware:usb_device) interface
* `hExt.serial` -- the serial interface on [hExt1](https://wiki.robocore.io/hardware:hext)
* hSensors 3 (see [the chapter about hSensors](#hsensors))
{% elif board(core2mini) %}
Husarion CORE2 mini has several serial interfaces:

* `Serial` -- the [USB-serial](https://wiki.robocore.io/hardware:usb_device) interface
* hSensors 3 (see [the chapter about hSensors](#hsensors))
{% endif %}

{% if port(stm32) %}
--seeAlso hSerial
{% endif %}

{% if port(stm32) %}
## Configuring the serial port

--example c++ serial_config.cpp func:hMain

The main `Serial` port is initialized by default with baudrate `460800` and no parity.
If you want to use other ports, you need to configure them manually using the `init` function.

--seeAlso hSerial hSensor
{% endif %}
