@PORTS: stm32
@BOARDS: core2
@NAME: can_recv
@CATEGORY: Interfaces
#include <hFramework.h>

void hMain()
{
	CAN.init();

	CAN_frameRx frame;
	bool frameReceived = CAN.waitFrame(frame, 100);
	if (frameReceived)
	{
		// access the frame in `frame` variable
	}
}
#####
@PORTS: stm32
@BOARDS: core2
@NAME: can_send
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
	tx.DLC = 2;
	tx.data[0] = 0xab;
	tx.data[1] = sys.getRefTime();
	CAN.sendFrame(tx);
}
