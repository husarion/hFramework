/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HGPIO_H_
#define __HGPIO_H_

namespace hFramework {
class hGPIO
{
protected:
    int nr;
    bool value;

public:
    hGPIO(int nr);

    void setOut();
    void setIn();
    bool read();
    void write(bool value);
    void toggle();
};

extern hGPIO gpio4;
extern hGPIO gpio5;

extern hGPIO gpio16;
extern hGPIO gpio17;
}

#endif
