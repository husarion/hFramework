@PORTS: stm32
@BOARDS: robocore,core2,core2mini
@NAME: motor_simple
@CATEGORY: Motors
#include <hFramework.h>

void hMain()
{
	sys.setLogDev(&Serial);

	while (true)
	{
		hMot1.setPower(500);
		sys.delay(1000);
		hMot1.setPower(-500);
		sys.delay(1000);
	}
}
