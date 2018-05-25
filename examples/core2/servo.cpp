#include <hFramework.h>
#include <cmath>

void hMain()
{
	hServoModule.enablePower();  // enable servos
	hServoModule.setVoltage5V(); // set power line voltage for all servos to about 5 [V].

	hServoModule.servo1.calibrate(-90, 700, 90, 1500); // calibrate servo with params minAngle, minWidth, maxAngle, maxWidth

	while (true)
	{
		int time = sys.getRefTime();
		float pos = sinf(time / 3000.0f * 2 * M_PI);
		hServoModule.servo1.rotAbs(pos * 90.0f); // wave the servo there and back using `sin` function
	}
}