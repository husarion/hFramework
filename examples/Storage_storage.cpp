@PORTS: stm32
@BOARDS: core2,core2mini
@NAME: storage
@DESC: This example shows how to use storage object implemented in hFramework.
@CATEGORY: Storage
#include <hFramework.h>

void hMain()
{
	sys.setSysLogDev(&devNull);							//turn off system logs on Serial
	sys.setLogDev(&Serial);								// setting USB-serial as a default printf output
	Serial.init(19200, Parity::None, StopBits::One);	// configure USB-serial with baudrate == 19200, none parity and with one stop bit

	sys.getStorage().store<int>(0, 42);		// Store a number 42 at location 0
	int answer;							   	// Creating variable for storing value from memory
	sys.getStorage().load<int>(0, answer); 	// Load a number from location 0
	printf("Stored value: %d\n\r",answer);	// Print readed number

	const char *str = "foo";			 	// Creating string containing "foo"
	sys.getStorage().store(4, str, 3);   	// Store a string of length 3 at location 4
	char newstr[4] = {0};				 	// Creating empty string of length 4
	sys.getStorage().load(4, newstr, 3); 	// Load a string of length 3 from location 4
	printf("Stored string: %s\n\r",newstr); // Print readed string
	
}