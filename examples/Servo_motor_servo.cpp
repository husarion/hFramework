@PORTS: stm32
@BOARDS: robocore,core2,core2mini
@NAME: motor_servo
@CATEGORY: Servo
#include <hFramework.h>

void hMain()
{
	IServo& servo = hMot1.useAsServo();
	// minAngle, minWidth, maxAngle, maxWidth
	servo.calibrate(-90, 700, 90, 1500);

	while (true)
	{
		int time = sys.getRefTime();
		float pos = sinf(time / 3000.0f * 2 * M_PI);
		servo.rotAbs(pos * 90.0f);
	}
}
