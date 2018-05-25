#include <hFramework.h>

void hMain()
{
	while (true)
	{
		hMot1.setPower(500);  // set hMot1 power to 50%
		sys.delay(1000);      
		hMot1.setPower(-500); // set hMot1 power to 50% in reverse direction
		sys.delay(1000);      
	}
}
