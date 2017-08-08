@PORTS: stm32
@BOARDS: core2,core2mini,robocore
@NAME: storage
@DESC: This example shows how to use storage object implemented in hFramework.
@CATEGORY: Storage
#include <hFramework.h>

void hMain()
{
	sys.setSysLogDev(&devNull);							//turn off system logs on Serial
	sys.setLogDev(&Serial);								// setting USB-serial as a default printf output
	Serial.init(19200, Parity::None, StopBits::One);	// configure USB-serial with baudrate == 19200, none parity and with one stop bit

	sys.getStorage().store<int>(0, 42);		// store a number 42 at location 0
	int answer;							   	// creating variable for storing value from memory
	sys.getStorage().load<int>(0, answer); 	// load a number from location 0
	printf("Stored value: %d\n\r",answer);	// print readed number

	const char *str = "foo";			 	// creating string containing "foo"
	sys.getStorage().store(4, str, 3);   	// store a string of length 3 at location 4
	char newstr[4] = {0};				 	// creating empty string of length 4
	sys.getStorage().load(4, newstr, 3); 	// load a string of length 3 from location 4
	printf("Stored string: %s\n\r",newstr); // print readed string
	
}