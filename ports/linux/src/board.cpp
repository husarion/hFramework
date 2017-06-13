#include <hFramework.h>
#include <fstream>

namespace hFramework {
hMutex mutex;
bool initialized = false;

enum BoardModel {
    RaspberryPi,
    Tinkerboard
};

BoardModel boardModel;

void initBoard() {
    hMutexGuard guard (mutex);
    if (initialized) return;

    std::ifstream f ("/proc/device-tree/compatible");
    std::string model;
    std::getline(f, model);

    if (model.find("rk3288") != -1) {
        boardModel = Tinkerboard;

        gpio3.nr = 252;
        gpio5.nr = 253;
        gpio7.nr = 17;
        gpio8.nr = 161;
        gpio10.nr = 160;
        gpio11.nr = 164;
        gpio12.nr = 184;
        gpio13.nr = 166;
    } else {
        boardModel = RaspberryPi;

        gpio3.nr = 2;
        gpio5.nr = 3;
        gpio7.nr = 4;
        gpio8.nr = 14;
        gpio10.nr = 15;
        gpio11.nr = 17;
        gpio12.nr = 18;
        gpio13.nr = 27;
    }

    initialized = true;
}

bool hBoardSerial::init(uint32_t baudrate, Parity parity) {
    initBoard();

    switch (boardModel) {
    case RaspberryPi:
        path = "/dev/serial0";
        break;
    case Tinkerboard:
        path = "/dev/ttyS1";
        break;
    }

    this->hSerialFile::init(baudrate, parity);
}

hBoardSerial Serial1 (1);

hGPIO gpio3 (3);
hGPIO gpio5 (5);
hGPIO gpio7 (7);
hGPIO gpio8 (8);
hGPIO gpio10 (10);
hGPIO gpio11 (11);
hGPIO gpio12 (12);
hGPIO gpio13 (13);

}
