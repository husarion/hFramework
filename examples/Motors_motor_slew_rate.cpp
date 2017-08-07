@PORTS: stm32
@BOARDS: core2,core2mini
@NAME: motor_slew_rate
@CATEGORY: Motors
#include <hFramework.h>

void hMain()
{
	sys.taskCreate([]() // This creates a task that will control two motors
				   {
					   hMot2.setSlewRate(0.01); // Set motor2 rotation rise time to 10s
					   for (;;)					// Task loop
					   {
						   hMot1.setPower(400);  // Set motor1 power to 500
						   hMot2.setPower(400);  // Set motor2 power to 500
						   sys.delay(4000);		 // Sleep for 4000 miliseconds
						   hMot1.setPower(-400); // Set motor power to 500 in reverse direction
						   hMot2.setPower(-400); // Set motor power to 500 in reverse direction
						   sys.delay(4000);		 // Sleep for 4000 miliseconds
					   }
				   });

	sys.delay(10000);	 // Sleep for 10000 miliseconds
	hMot2.setSlewRate(0); // Set motor2 rotation rise time to 0s
}
