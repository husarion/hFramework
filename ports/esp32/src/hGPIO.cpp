/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hGPIO.h"

extern "C" {
#include "driver/gpio.h"
}

namespace hFramework {

hGPIO::hGPIO(int nr): nr(nr), value(false) {
}

void hGPIO::setOut() {
    gpio_config_t io_conf;

    io_conf.intr_type = (gpio_int_type_t)GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = 1<<nr;

    io_conf.pull_down_en = (gpio_pulldown_t)0;
    io_conf.pull_up_en = (gpio_pullup_t)0;

    gpio_config(&io_conf);
}

void hGPIO::setIn() {
    gpio_config_t io_conf;

    io_conf.intr_type = (gpio_int_type_t)GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = 1<<nr;

    io_conf.pull_down_en = (gpio_pulldown_t)0;
    io_conf.pull_up_en = (gpio_pullup_t)0;

    gpio_config(&io_conf);
}

bool hGPIO::read() {
    return gpio_get_level((gpio_num_t)nr) != 0;
}

void hGPIO::write(bool value) {
    this->value = value;
    gpio_set_level((gpio_num_t)nr, value ? 1 : 0);
}

void hGPIO::toggle() {
    write(!value);
}

hGPIO gpio4 (4);
hGPIO gpio5 (5);

hGPIO gpio16 (16);
hGPIO gpio17 (17);

}
