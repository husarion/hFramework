@PORTS: stm32
@BOARDS: core2,core2mini,robocore
@NAME: motor_angle
@DESC: This example shows how to control the motor rotation angle
@CATEGORY: Motors

#include <hFramework.h>

void hMain()
{
    hMot3.setEncoderPolarity(Polarity::Reversed);  //changing encoder polarity (Polarity::Normal is default)
        hMot3.setMotorPolarity(Polarity::Reversed);  //changing motor polarity
    char c;
    while (1)
    {  
        if (Serial.available() > 0) { // checking Serial availability
            c = Serial.getch();
            if(c == 'a') {
                hMot3.rotRel(500, 200, false, INFINITE); //rotate by 500 ticks relatively to the current position, with 20% power
                hLED1.toggle();
            }
            if(c == 's') {
                hMot3.rotRel(-500, 200, true, INFINITE); //relative rotate 500 encoder ticks left with 20% of power with blocking task
                hLED1.toggle();
            }
            if(c == 'd') {
                hMot3.rotAbs(0, 200, true, INFINITE); //rotate to "0" ticks absolute position, and block program until task finishes
                hLED1.toggle();
            }
            sys.delay(200);
        }
    }
}
