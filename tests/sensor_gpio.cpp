#include <hFramework.h>

using namespace hFramework;

hSensor* s[] = { &hSens1, &hSens2, &hSens3, &hSens4, &hSens5 };

void hMain(void)
{
	for (int i = 0; i < 5; i++)
	{
		s[i]->selectGPIO();
		s[i]->getPin1().setOut();
		s[i]->getPin2().setOut();
	}
	
	for (;;)
	{
		for (int i = 0; i < 5; i++)
		{
			s[i]->getPin1().toggle();
			s[i]->getPin2().toggle();
		}
		sys.delay(1000);
	}
}
