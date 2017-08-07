@PORTS: stm32
@BOARDS: core2,core2mini
@NAME: motor_pid
@CATEGORY: Motors
#include <hFramework.h>

hPIDRegulator pidReg, pidReg2; // creating PID regulator objects

void hMain()
{
	pidReg.setScale(1);		 // setting Scale for PID regulator
	pidReg.setKP(40.0);		 // setting the proportional coefficient
	pidReg.setKI(0.05);		 // setting the integral coefficient
	pidReg.setKD(1000);		 // setting the derivative coefficient
	pidReg.dtMs = 5;		 // setting time increment (Delta T)
	pidReg.stableRange = 10; // setting the value of stable range
	pidReg.stableTimes = 3;  // setting the valu of stable time

	pidReg2 = pidReg; // Copy pid instances

	hMot1.attachPositionRegulator(pidReg);  // attaching pidReg regulator to motor1
	hMot2.attachPositionRegulator(pidReg2); // attaching pidReg2 regulator to motor2

	sys.taskCreate([]() {
		while (true)
		{
			sys.delay(50);
			Serial.printf("encoder ticks: %5d %5d\r\n", hMot1.getEncoderCnt(), hMot2.getEncoderCnt());
		}
	});

	sys.taskCreate([]() {
		while (true)
		{
			hMot1.rotRel(360, 200, 1); // rotate relatively hMot1 360 tics right with 200 power and block task until finished
		}
	});

	sys.taskCreate([]() {
		while (true)
		{
			hMot2.rotAbs(360, 200, 1); // rotate absolutely hMot2 to 360 tics with 200 power and block task until finished
			hMot2.rotAbs(0, 200, 1);   // rotate absolutely hMot2 to 0 tics with 200 power and block task until finished
		}
	});
}
