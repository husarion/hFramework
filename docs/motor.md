{% if port(stm32) %}
# Motor


[hMotor](https://wiki.robocore.io/hardware:hmotor) is an interface for DC motors with quadrature encoder (optionally).

--example c++ motor_simple.cpp func:hMain

%%%hMotor::setPower%%% is the lowest-level function which can be used to directly control the motor power.

--seeAlso hMotor

## Using the encoder interface

### rotRel and rotAbs

--example c++ motor_angle.cpp func:hMain

You can also use %%%hMotor::rotRel%%% and %%%hMotor::rotAbs%%% to directly control the motor angle. Just be aware that these methods block until the requested angle is reached by the motor

<br clear=both />

### getEncoderCnt

--example c++ motor.cpp stripheaders:

If your motor has an encoder interface you can check number of encoder ticks using %%%hMotor::getEncoderCnt%%% function

--seeAlso hMotor

## Using servos connected to hMotors

--example c++ motor_servo.cpp func:hMain

Before using servo, you should configure it using %%%IServo::calibrate%%% method.
Different servos has different pulse lengths, that correspond to the actual value. With this function you can calibrate the servo, so the %%%IServo::rotAbs%%% function acts the same way in each servo.

--seeAlso hMotor IServo
{% endif %}
