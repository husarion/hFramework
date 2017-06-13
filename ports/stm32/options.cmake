get_filename_component(PORT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

set(prefix arm-none-eabi-)
if (WIN32)
  set(exesuffix .exe)
else()
  set(exesuffix "")
endif()

disable_compiler_detection()

set(port_flags "-fno-builtin-printf -fno-omit-frame-pointer -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard")
set(port_lflags "-T${HFRAMEWORK_DIR_Q}/stm32f4_linker.ld")

add_definitions(-DUSE_USB_OTG_FS -DUSE_STDPERIPH_DRIVER -DSTM32F4XX -DSTM32F4 -DSTM32F407ZG -D__FPU_USED -DARM_MATH_CM4 -D__FPU_PRESENT)

include_directories(${PORT_DIR}/include)
include_directories(${PORT_DIR}/src)
include_directories(${PORT_DIR}/src/hPeriph)

include_directories("${PORT_DIR}/src/hUSB/usb/")
include_directories("${PORT_DIR}/src/hUSB/")

macro(add_hexecutable_port name)
  add_custom_target("${name}.bin" ALL
    DEPENDS "${name}.elf"
    COMMAND ${prefix}objcopy -O binary "${OUTDIR}${name}.elf" "${OUTDIR}${name}.bin")

  add_custom_target("${name}.hex" ALL
    DEPENDS "${name}.elf"
    COMMAND ${prefix}objcopy -O ihex "${OUTDIR}${name}.elf" "${OUTDIR}${name}.hex")

  add_custom_target("flash_${name}"
    DEPENDS "${OUTDIR}${name}.hex"
    USES_TERMINAL
    COMMAND ${HFRAMEWORK_PATH}/tools/${TOOLS_ARCH_NAME}/core2-flasher "${OUTDIR}${name}.hex")
endmacro()
