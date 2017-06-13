#include <hFramework.h>

using namespace hFramework;

hMotor* s[] = { &hMot1, &hMot2, &hMot3, &hMot4, &hMot5, &hMot6 };

void a()
{
	for (;;)
	{
		int m1 = hMot1.getEncoderCnt();
		int m2 = hMot2.getEncoderCnt();
		int m3 = hMot3.getEncoderCnt();
		int m4 = hMot4.getEncoderCnt();
		int m5 = hMot5.getEncoderCnt();
		int m6 = hMot6.getEncoderCnt();
		sys.log("%5d %5d %5d %5d %5d %5d\r\n", m1, m2, m3, m4, m5, m6);
		sys.delay(200);
	}
}

void hMain(void)
{
	sys.setLogDev(&Serial);
	sys.taskCreate(a);
	for (;;)
	{
		for (int i = 0; i < 6; i++)
			s[i]->setPower(200);
		sys.delay(1000);
		for (int i = 0; i < 6; i++)
			s[i]->setPower(-250);
		sys.delay(1000);
	}
}
