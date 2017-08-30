@PORTS: stm32
@BOARDS: core2,robocore
@NAME: can_send
@DESC: The example shows how to transmit frames with the CAN interface
@CATEGORY: Interfaces
#include <hFramework.h>

void hMain()
{
	CAN.init();

	CAN_frameTx tx;
	tx.StdId = 0;
	tx.ExtId = 0;
	tx.IDE = 0;
	tx.RTR = 0;
	tx.DLC = 2; // Data Length Code (in bytes)
	tx.data[0] = 0xab;
	tx.data[1] = (int)sys.getRefTime();
	CAN.sendFrame(tx);
}
// see more about CAN frame on Wikipedia: https://en.wikipedia.org/wiki/CAN_bus#Data_frame

#####
@PORTS: stm32
@BOARDS: core2,robocore
@NAME: can_receive
@DESC: The example shows how to receive frames with the CAN interface
@CATEGORY: Interfaces
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
