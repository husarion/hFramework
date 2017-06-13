@PORTS: stm32
@BOARDS: core2
@NAME: servo_module
@CATEGORY: Servo
#include <hFramework.h>
#include <cmath>

void hMain()
{
	hServo.enablePower();
	hServo.setVoltage5V();

	IServo& s1 = hServo.servo1;
	// minAngle, minWidth, maxAngle, maxWidth
	s1.calibrate(-90, 700, 90, 1500);

	while (true)
	{
		// wave the servo there and back using `sin` function
		int time = sys.getRefTime();
		float pos = sinf(time / 3000.0f * 2 * M_PI);
		s1.rotAbs(pos * 90.0f);
	}
}
@PORTS: stm32
@BOARDS: core2mini
@NAME: servo_module
@CATEGORY: Servo
#include <hFramework.h>
#include <cmath>

void hMain()
{
	hServo.enablePower();


	IServo& s1 = hServo.servo1;
	// minAngle, minWidth, maxAngle, maxWidth
	s1.calibrate(-90, 700, 90, 1500);

	while (true)
	{
		// wave the servo there and back using `sin` function
		int time = sys.getRefTime();
		float pos = sinf(time / 3000.0f * 2 * M_PI);
		s1.rotAbs(pos * 90.0f);
	}
}
