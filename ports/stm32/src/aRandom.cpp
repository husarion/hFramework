#include <Arduino.h>
#include <stdlib.h>

long random(long max)
{
	return (long)rand() % max;
}

long random(long min, long max)
{
	return (long)rand() % (max - min) + min;
}
