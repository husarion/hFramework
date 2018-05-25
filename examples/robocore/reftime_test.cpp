#include <cstdint>
#include <cassert>

uint64_t lastTicks = 0;
uint64_t rawticks = 0;

uint64_t getRefTime()
{
    uint64_t ticks32 = rawticks;
    uint64_t newTicks = (lastTicks & 0xFFFFFFFF00000000ull) | ticks32;
    if (ticks32 < (lastTicks & 0xFFFFFFFFull)) {
        newTicks += 0x100000000ull;
    }
    lastTicks = newTicks;
    return lastTicks;
}

int main() {
    assert(getRefTime()==0);
    rawticks = 10;
    assert(getRefTime()==10);
    assert(getRefTime()==10);
    rawticks = 20;
    assert(getRefTime()==20);
    rawticks = 0x100000000ull - 20;
    assert(getRefTime()==0x100000000ull - 20);
    rawticks = 10;
    assert(getRefTime()==0x100000000ull + 10);
    rawticks = 20;
    assert(getRefTime()==0x100000000ull + 20);
}
