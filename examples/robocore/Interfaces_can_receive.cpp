#include <hFramework.h>

void hMain()
{
	CAN.init();

	CAN_frameRx frame;
	bool frameReceived = CAN.waitFrame(frame, 100);  // receive a frame with 100ms timeout
	if (frameReceived)
	{
		// access the frame in `frame` variable
		if(frame.data[0] == 0xab) printf("Received system time: %d\r\n", frame.data[1]); 
	}
}
// see more about CAN frame on Wikipedia: https://en.wikipedia.org/wiki/CAN_bus#Data_frame