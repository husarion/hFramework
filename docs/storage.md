# Persistent storage

--example c++ storage.cpp func:hMain

Sometimes you need to store some information is a persistent storage that should be available between RoboCORE resets. hStorage provides such space. There are 8 KiB available - the last 1 KiB is reserved for hFramework internal configuration.

All data is stored in virtual 16-bit address space. Addresses from 0 to 7168 bytes are available for user, others are reserved for internal hFramework use.

Persistent storage is implemented using EEPROM emulation, which works on two dedicated flash pages.

--seeAlso hStorage
