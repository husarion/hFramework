{% if port(stm32) %}
# GPIO

--example c++ gpio.cpp func:hMain

RoboCORE has a lot of GPIO ports (General Purpose Input/Output). These GPIO ports are available via:

* [two interfaces called hExts](https://wiki.robocore.io/hardware:hext)
* [hSensor ports](https://wiki.robocore.io/hardware:hsensor)

--seeAlso hSensor hGPIO

## GPIO settings

--example c++ gpio_inout.cpp func:hMain
--example c++ gpio_inout.cpp func:hMain2

GPIO ports can be set to be either input or output ports. There are several modes combining push/pull resistors:

* `setOut` -- push up for 1, pull down for 0
* `setOutOD` -- output in open-drain mode (low state for 0, undefined/high impedance for 1)
* `setOutOD_pu` -- output in open-drain mode and with push-up resistor (low state for 0, "weak high" state for 1)
* `setOutOD_pd` -- output in open-drain mode and with pull-down resistor (low state for 0, "weak low" state for 1)
* `setIn` -- input (1 for high state, 0 for low state)
* `setIn_pd` -- input with pull down resistor (1 for high state, 0 for low and undefined states)
* `setIn_pu` -- input with push up resistor (1 for high and undefined states, 0 for low state)

--seeAlso hGPIO

## Analog input

--example c++ adc.cpp func:hMain

Some ports on RoboCORE have ADC capability (i.e. they can read analog voltages) -- hSens4.pin2, hExt1.pin7 and hExt2.pin7.

--seeAlso hGPIO_adc
{% endif %}
