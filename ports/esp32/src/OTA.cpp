#include <hFramework.h>
#include <cstring>
#include <utility>
#include "esp_ota_ops.h"

namespace hFramework {
namespace OTA {

const esp_partition_t* getOtaPartition()
{
    const esp_partition_t* bootPartition = esp_ota_get_boot_partition();
    const char* newname;

    if (bootPartition == nullptr || strcmp("ota_0", bootPartition->label) == 0) {
        fprintf(stderr, "OTA: currently running from partition 'ota_0'\n");
        newname = "ota_1";
    } else if (strcmp("ota_1", bootPartition->label) == 0) {
        fprintf(stderr, "OTA: currently running from partition 'ota_1'\n");
        newname = "ota_0";
    } else {
        fprintf(stderr, "OTA: invalid boot partition\n");
        return nullptr;
    }

    return esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, newname);
}


bool run(uint32_t crc32, int length, hStreamDev& dev) {
    const esp_partition_t* otaPart = getOtaPartition();
    esp_ota_handle_t handle;
    if (esp_ota_begin(otaPart, 0, &handle) != 0) {
        fprintf(stderr, "OTA: esp_ota_begin error\n");
        return false;
    }

    const int maxChunkSize = 4096;
    char* chunkBuffer = (char*)malloc(maxChunkSize);

    while (length > 0) {
        int chunkSize = std::min(length, maxChunkSize);
        if (dev.readAll(chunkBuffer, chunkSize) != chunkSize) {
            fprintf(stderr, "OTA: steam read failed\n");
            return false;
        }
        length -= chunkSize;
        if (esp_ota_write(handle, chunkBuffer, chunkSize) != 0) {
            fprintf(stderr, "OTA: esp_ota_write error\n");
            return false;
        }
    }

    if (esp_ota_end(handle) != 0) {
        fprintf(stderr, "OTA: esp_ota_end failed\n");
        return false;
    }

    // TODO: check crc32

    if (esp_ota_set_boot_partition(otaPart) != 0) {
        fprintf(stderr, "OTA: esp_ota_set_boot_partition\n");
        return false;
    }

    fprintf(stderr, "OTA: finished successfully\n");
    return true;
}

}
}
