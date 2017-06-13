{% if port(stm32) %}
# hSensors

RoboCORE is equipped with 5 [hSensor](https://wiki.robocore.io/hardware:hsensor) ports. They meant to be used with many different sensors, such as spatial orientation sensors (MPU9150-module for RoboCORE), light sensors, sound sensors, limit switches and many others.

The hSensor class provides the low level functions (GPIO, serial port, ADC and I2C). You can also pass the hSensor instance to a library providing support for a specific sensor (for example, to one of the modules from [sensors support library](#sensors-support)).

--seeAlso hSensor
{% endif %}
