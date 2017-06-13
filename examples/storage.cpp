@NAME: storage
@CATEGORY: Storage
#include <hFramework.h>

void hMain()
{
	// store a number at location 0
	sys.getStorage().store<int>(0, 42);
	// load a number from location 0
	int answer;
	sys.getStorage().load<int>(0, answer);

	// store a string at location 4
	const char* str = "foo";
	sys.getStorage().store(4, str, 3);
	// load a string from location 4
	char newstr[4] = {0};
	sys.getStorage().load(4, newstr, 3);
}
