/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <ESPStorage.h>

namespace hFramework {
ESPStorage ESPStorage::open(const char* ns) {
    nvs_handle handle;
    ESP_ERROR_CHECK(nvs_open(ns, NVS_READWRITE, &handle));
    return ESPStorage{handle};
}
void ESPStorage::write(const char* name, const void* data, int len) {
    ESP_ERROR_CHECK(nvs_set_blob(handle, name, data, len));
    ESP_ERROR_CHECK(nvs_commit(handle));
}
void ESPStorage::read(const char* name, void* data, int& len) {
    int ret = nvs_get_blob(handle, name, data, (size_t*)&len);
    if (ret == ESP_ERR_NVS_NOT_FOUND) {
        len = -1;
        return;
    }
    ESP_ERROR_CHECK(ret);
}

}
