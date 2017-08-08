@PORTS: stm32
@BOARDS: core2,core2mini,robocore
@NAME: motor_servo
@CATEGORY: Servo
#include <hFramework.h>

void hMain()
{
	IServo &servo = hMot1.useAsServo();  // enable usage of hMot1 as servo
	servo.calibrate(-90, 700, 90, 1500); // calibrate servo -> minAngle, minWidth, maxAngle, maxWidth

	while (true) 
	{
		int time = sys.getRefTime();				
		float pos = sinf(time / 3000.0f * 2 * M_PI); // calculate successive sinusoidal values depend on time
		servo.rotAbs(pos * 90.0f);					 // rotate motor absolutely to set value
	}
}
