/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <cstdint>

namespace hFramework {
struct ESPStorage {
    uint32_t handle;

    static ESPStorage open(const char* ns);
    void write(const char* name, const void* data, int len);
    void read(const char* name, void* data, int& len);
};
}
