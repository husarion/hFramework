get_filename_component(PORT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

set(prefix xtensa-esp32-elf-)

disable_compiler_detection()
set(${prefix})

set(esp32_libs "-lapp_update -lbootloader_support -lbt -lcoap -lcxx -ldriver -lesp32 -lcore -lrtc -lrtc_clk -lphy -lcoexist -lnet80211 -lpp -lwpa -lsmartconfig -lcoexist -lwps -lwpa2 -lethernet -lexpat -lfatfs -lfreertos -ljson -llog -llwip -lmbedtls -lmdns -lmicro-ecc -lnewlib -lnghttp -lnvs_flash -lopenssl -lsdmmc -lspi_flash -ltcpip_adapter -lulp -lvfs -lwpa_supplicant -lxtensa-debug-module -lhal -lc -lm -lhFramework")

set(port_flags "")
set(port_lflags "-nostdlib -Wl,--gc-sections -Wl,-static -Wl,--start-group -u uxTopUsedPriority -u call_user_start_cpu0 -u __cxa_guard_dummy ${esp32_libs} -lgcc -lstdc++ -T esp32_out.ld -T esp32.common.ld -T esp32.rom.ld -T esp32.peripherals.ld -Wl,--end-group -Wl,-EL")

add_definitions(-fdata-sections -fstrict-volatile-bitfields -mlongcalls -nostdlib -DWITH_POSIX -DMBEDTLS_CONFIG_FILE='"mbedtls/esp_config.h"' -DHAVE_CONFIG_H -DESP_PLATFORM -DIDF_VER="v2.0-rc1-154-g3b8c9a4" -fno-exceptions)

include_directories(${PORT_DIR}/include)

function(add_hexecutable_port name)
  add_custom_target("${name}.bin" ALL
    DEPENDS "${name}.elf"
    COMMAND python ${HFRAMEWORK_PATH}/third-party/esp-idf/components/esptool_py/esptool/esptool.py --chip esp32 elf2image --flash_mode "dio" --flash_freq "40m" --flash_size "2MB"  -o "${name}.bin" "${name}.elf")

  add_custom_target("flash_${name}"
    DEPENDS "${OUTDIR}${name}.bin"
    USES_TERMINAL
    COMMAND python ${HFRAMEWORK_PATH}/third-party/esp-idf/components/esptool_py/esptool/esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 115200 --before default_reset --after hard_reset write_flash -u --flash_mode dio --flash_freq 40m --flash_size detect 0x1000 ${HFRAMEWORK_PATH}/build/esp32_esp32_1.0.0/bootloader.bin 0x10000 ${name}.bin 0x8000 ${HFRAMEWORK_PATH}/build/esp32_esp32_1.0.0/partitions_ota.bin)
endfunction()
