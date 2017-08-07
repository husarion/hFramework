@PORTS: stm32
@BOARDS: core2,core2mini
@NAME: motor_angle
@CATEGORY: Motors

#include <hFramework.h>

void hMain()
{
	char c;
	while (1)
	{
		c = Serial.getch();
		if(c == 'a') {
			hMot3.rotRel(500, 200, false, INFINITE); //relative rotate 500 encoder ticks right with 20% of power without blocking task
			hLED1.toggle();
		}
		if(c == 's') {
			hMot3.rotRel(-500, 200, true, INFINITE); //relative rotate 500 encoder ticks left with 20% of power with blocking task
			hLED1.toggle();
		}
		if(c == 'd') {
			hMot3.rotAbs(0, 200, true, INFINITE); //absolute rotate to 0 ticks position with 20% of power and with blocking task
			hLED1.toggle();
		}
		sys.delay(200);
	}
}
