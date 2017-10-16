#include <hFramework.h>
#include "Arduino.h"

hFramework::ArduinoSerial Serial(hFramework::Serial);

unsigned long hFramework::millis(void) {
   return sys.getRefTime();
}
  
unsigned long hFramework::micros(void) {
   return sys.getRefTime()*1000;
}
  
void hFramework::delay(int ms) {
    sys.delay(ms);
}
  
void hFramework::delayMicroseconds(unsigned int us) {
    sys.delayUs(us);
}

void hFramework::memset(const void* ptr, int value, size_t size){
    unsigned char const *a=*(( unsigned char const **)ptr);
    unsigned char const b = (unsigned char)value;
    for(size_t i = 0 ; i<size; i++)
	{
        a = &b;
        a += 1;
	}
}

int hFramework::memcmp( const void * ptr1, const void * ptr2, size_t num )
{
    int const *a=*(( int const **)ptr1);
    int const *b=*(( int const **)ptr2);
	for(size_t i = 0; i< num ; i++)
	{ 
		if(a[i] > b[i])
			return 1;
		if(a[i] < b[i])
			return 2;
	}
	return 0;
}