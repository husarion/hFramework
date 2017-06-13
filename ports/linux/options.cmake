get_filename_component(PORT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

if(NOT DEFINED ${BOARD_TYPE})
  message(FATAL_ERRROR "BOARD_TYPE missing")
endif()

if(${BOARD_TYPE} STREQUAL "x86generic")
  set(ARCH x86generic)
elseif(${BOARD_TYPE} STREQUAL "rpi")
  set(ARCH arm)
else()
  message(FATAL_ERRROR "invalid board type ${BOARD_TYPE} (Linux port)")
endif()

set(port_lflags "")

if(${ARCH} STREQUAL arm)
  set(prefix "arm-linux-gnueabihf-")
  disable_compiler_detection()
else(${ARCH} STREQUAL x86generic)
  set(prefix "")
  add_definitions("-m32")
  set(port_lflags "-m32")
  disable_compiler_detection()
else()
  message(FATAL_ERRROR "invalid architecture ${ARCH}")
endif()

set(CMAKE_C_COMPILER ${prefix}gcc)
set(CMAKE_ASM_COMPILER ${prefix}gcc)
set(CMAKE_CXX_COMPILER ${prefix}g++)

set(port_flags "")
set(port_lflags "${port_lflags} -pthread -lutil -lrt -lssl -lcrypto")

include_directories(${PORT_DIR}/include)
include_directories(${PORT_DIR}/src)

set(CMAKE_AR ${prefix}ar)
set(CMAKE_RANLIB ${prefix}ranlib)

macro(add_hexecutable_port name)
# no additional targets
endmacro()
