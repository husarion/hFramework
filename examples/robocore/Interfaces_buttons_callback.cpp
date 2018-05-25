#include <hFramework.h>

void onPress() // instruction executed by clicking the button 
{
	hLED1.on();
	hLED2.toggle();
	hMot1.setPower(500);
}

void onRelease() // instruction executed by releasing the button 
{
	hLED1.off();
	hMot1.setPower(0);
}

void hMain()
{
	for (;;)
	{
		hBtn1.setOnPressHandler(onPress);	 // onPress will be invoked when hBtn1 is pressed
		hBtn1.setOnReleaseHandler(onRelease); // onRelease will be invoked when hBtn1 is released
		sys.delay(100);
	}
}
