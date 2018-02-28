get_filename_component(PORT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

set(prefix xtensa-esp32-elf-)

disable_compiler_detection()
set(${prefix})

set(esp32_libs "-lapp_update -lbootloader_support -lbt -lcoap -lcxx -ldriver -lesp32 -lcore -lrtc -lsoc -lphy -lcoexist -lnet80211 -lpp -lwpa -lsmartconfig -lcoexist -lwps -lwpa2 -lethernet -lexpat -lfatfs -lfreertos -ljson -llog -llwip -lmbedtls -lmdns -lmicro-ecc -lnewlib -lnghttp -lnvs_flash -lopenssl -lsdmmc -lspi_flash -ltcpip_adapter -lulp -lvfs -lwpa_supplicant -lxtensa-debug-module -lhal -lc -lconsole -lm -lgcc -lstdc++ -lgcov -lnewlib -lheap -lapp_trace -lpthread -llibsodium -lhFramework")

set(port_flags "")
set(port_lflags "-nostdlib -Wl,--gc-sections -Wl,-static -Wl,--start-group <LINK_LIBRARIES> -u uxTopUsedPriority -u call_user_start_cpu0 -u __cxa_guard_dummy ${esp32_libs} -T esp32_out.ld -u ld_include_panic_highint_hdl -T esp32.common.ld -T esp32.rom.ld -T esp32.peripherals.ld -T esp32.rom.spiram_incompatible_fns.ld -Wl,--end-group -Wl,-EL")

add_definitions(-fdata-sections -fstrict-volatile-bitfields -mlongcalls -nostdlib -DWITH_POSIX -DMBEDTLS_CONFIG_FILE='"mbedtls/esp_config.h"' -DHAVE_CONFIG_H -DESP_PLATFORM -DIDF_VER="v2.0-rc1-154-g3b8c9a4" -fno-exceptions)

if (NOT IDF_PATH)
  #message(FATAL_ERROR "IDF_PATH variable missing")
  set(IDF_PATH ${HFRAMEWORK_PATH}/third-party/esp-idf)
endif()

include_directories("${HFRAMEWORK_PATH}/ports/esp32/build-sdk/build/include")

include_directories("${IDF_PATH}/components/vfs/include/")
include_directories("${IDF_PATH}/components/soc/include/")
include_directories("${IDF_PATH}/components/heap/include/")
include_directories("${IDF_PATH}/components/freertos/include/")
include_directories("${IDF_PATH}/components/esp32/include")
include_directories("${IDF_PATH}/components/tcpip_adapter/include")
include_directories("${IDF_PATH}/components/lwip/include/lwip")
include_directories("${IDF_PATH}/components/lwip/include/lwip/port")
include_directories("${IDF_PATH}/components/driver/include")
include_directories("${IDF_PATH}/components/nvs_flash/include")
include_directories("${IDF_PATH}/components/openssl/include")
include_directories("${IDF_PATH}/components/app_update/include")
include_directories("${IDF_PATH}/components/spi_flash/include")
include_directories("${IDF_PATH}/components/soc/esp32/include")
include_directories("${IDF_PATH}/components/libsodium/libsodium/src/libsodium/include/")
include_directories("${IDF_PATH}/components/expat/include/expat/")

include_directories(${PORT_DIR}/include)

function(add_hexecutable_port name)
  add_custom_target("${name}.bin" ALL
    DEPENDS "${name}.elf"
    COMMAND python ${IDF_PATH}/components/esptool_py/esptool/esptool.py --chip esp32 elf2image --flash_mode "dio" --flash_freq "40m" --flash_size "2MB"  -o "${name}.bin" "${name}.elf")

  add_custom_target("flash_${name}"
    DEPENDS "${OUTDIR}${name}.bin"
    USES_TERMINAL
    COMMAND python ${IDF_PATH}/components/esptool_py/esptool/esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 115200 --before default_reset --after hard_reset write_flash -u --flash_mode dio --flash_freq 40m --flash_size detect 0x1000 ${HFRAMEWORK_PATH}/build/esp32_esp32_1.0.0/bootloader.bin 0x10000 ${name}.bin 0x8000 ${HFRAMEWORK_PATH}/build/esp32_esp32_1.0.0/partitions_ota.bin)

  add_custom_target("monitor_${name}"
    DEPENDS "${OUTDIR}${name}.bin"
    USES_TERMINAL
    COMMAND ${IDF_PATH}/tools/idf_monitor.py --make ninja ${name}.elf)
endfunction()
