#include <hFramework.h>

void hMain()
{
	sys.taskCreate([]() { // this creates a task that will control two motors
		hMot2.setSlewRate(0.01); // set hMot2 rotation rise time to 10s
		for (;;) {
			hMot1.setPower(400);  // set hMot1 power to 400
			hMot2.setPower(400);  // set hMot2 power to 400
			hMot1.setPower(-400); // set motor power to 400 in reverse direction
			hMot2.setPower(-400); // set motor power to 400 in reverse direction
			sys.delay(4000);		 
		}
	});
	sys.delay(10000);	 
	hMot2.setSlewRate(0); // set hMot2 rotation rise time to 0s
}
