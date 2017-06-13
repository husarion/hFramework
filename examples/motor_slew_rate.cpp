@PORTS: stm32
@BOARDS: robocore,core2,core2mini
@NAME: motor_slew_rate
@CATEGORY: Motors
#include <hFramework.h>

void hMain()
{
	sys.taskCreate([]()
	{
		hMot2.setSlewRate(0.05);
		for (;;)
		{
			hMot1.setPower(400);
			hMot2.setPower(400);
			sys.delay(4000);
			hMot1.setPower(-400);
			hMot2.setPower(-400);
			sys.delay(4000);
		}
	});

	sys.delay(10000);
	hMot2.setSlewRate(0);
}
