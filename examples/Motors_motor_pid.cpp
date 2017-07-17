@PORTS: stm32
@BOARDS: robocore,core2,core2mini
@NAME: motor_pid
@CATEGORY: Motors
#include <hFramework.h>

hPIDRegulator pidReg, pidReg2;

void hMain()
{
	sys.setLogDev(&Serial);

	// hPID
	pidReg.setScale(1);
	pidReg.setKP(40.0);
	pidReg.setKI(0.05);
	pidReg.setKD(1000);
	// hRegulator
	pidReg.dtMs = 5;
	pidReg.stableRange = 10;
	pidReg.stableTimes = 3;

	// copy pid instances
	pidReg2 = pidReg;

	hMot1.attachPositionRegulator(pidReg);
	hMot2.attachPositionRegulator(pidReg2);

	sys.taskCreate([]()
	{
		for (;;)
		{
			sys.delay(50);
			printf("encoder ticks: %5d %5d\r\n", hMot1.getEncoderCnt(), hMot2.getEncoderCnt());
		}
	});

	sys.taskCreate([]()
	{
		while (true)
		{
			hMot1.rotRel(180, 1000, 1);
			sys.delay(300);
		}
	});

	sys.taskCreate([]()
	{
		while (true)
		{
			hMot2.rotAbs(180, 1000, 1);
			sys.delay(300);
			hMot2.rotAbs(0, 1000, 1);
			sys.delay(300);
		}
	});
}
