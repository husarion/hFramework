@PORTS: stm32
@BOARDS: robocore,core2,core2mini
@NAME: buttons_callback
@CATEGORY: Interfaces
#include <hFramework.h>

void onPress()
{
	LED1.on();
	hMot1.setPower(500);
}

void onRelease()
{
	LED1.off();
	hMot1.setPower(0);
}

void hMain()
{
	// onPress will be invoked when hBtn1 is pressed
	hBtn1.setOnPressHandler(onPress);

	// onRelease will be invoked when hBtn1 is released
	hBtn1.setOnReleaseHandler(onRelease);
}
