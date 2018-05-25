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


