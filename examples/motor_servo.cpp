@PORTS: stm32
@BOARDS: core2,core2mini
@NAME: motor_servo
@CATEGORY: Servo
#include <hFramework.h>

void hMain()
{
	IServo &servo = hMot1.useAsServo();  // Enable usage of hMot1 as servo
	servo.calibrate(-90, 700, 90, 1500); // Calibrate servo -> minAngle, minWidth, maxAngle, maxWidth

	while (true) 
	{
		int time = sys.getRefTime();				
		float pos = sinf(time / 3000.0f * 2 * M_PI); // Calculate successive sinusoidal values depend on time
		servo.rotAbs(pos * 90.0f);					 // Rotate motor absolutely to set value
	}
}
