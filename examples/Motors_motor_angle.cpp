@PORTS: stm32
@BOARDS: robocore,core2,core2mini
@NAME: motor_angle
@CATEGORY: Motors
#include <hFramework.h>

void hMain()
{
	// rotate 10 encoder ticks right
	hMot1.rotRel(10);
	// restore motor to original position
	hMot1.rotAbs(0);
}
