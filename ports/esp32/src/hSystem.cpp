/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hSystem.h"
#include <cstring>

extern "C" {
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
};

namespace hFramework {

uint32_t hSystem::getRandNr() {
    return (uint32_t)esp_random();
}

uint64_t hSystem::getSerialNum() {
    uint8_t mac[8];
    esp_efuse_read_mac(mac);
    uint64_t id;
    memcpy(&id, mac, 8);
    return id;
}

void hSystem::reset() {
    system_restart();
}

hSystem sys;

}
