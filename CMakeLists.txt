cmake_minimum_required(VERSION 2.6)
get_filename_component(CURRENT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

project(hFramework NONE)

option(RELEASE "Enable release build" OFF)
option(USE_PYTHON "Enable python support" OFF)

set(debug_flags "-DHFRAMEWORK_COMPILE -DSYSTEM_DEBUG -DUSB_DEBUG -DUSB_ADK_DEBUG -DUSB_CORE_DEBUG")

if(NOT RELEASE)
	set(debug_flags "${debug_flags} -DDEBUG")
endif()

include("${HFRAMEWORK_PATH}/hFrameworkPort.cmake")

include_directories("${CURRENT_DIR}/include/")
include_directories("${CMAKE_BINARY_DIR}")

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(HFRAMEWORK_FILES

  src/Other/hPIDRegulator.cpp

  src/IServo.cpp
  src/II2C.cpp

  src/hSystem/hEventBits.cpp
  src/hSystem/main.cpp

  ${PORT_DIR}/src/hSystem.cpp

  src/Other/hPID.cpp
  src/Other/hPrintf.cpp
  src/Other/hPrintfDev.cpp
  src/Other/hScriptingLanguage.cpp
  src/Other/hSoftwareI2C.cpp
  src/Other/hStreamDev.cpp
  src/Other/hUtils.cpp
  src/Other/myprintf.cpp
  src/Other/hMath.cpp
)

if(${BOARD_TYPE} STREQUAL "core2")
  MESSAGE ("Compilation for CORE2")
  set(HFRAMEWORK_FILES

  ${HFRAMEWORK_FILES}
  src/panther/Panther.cpp
  src/rosbot/ROSbot.cpp
  src/rosbot/wheel.cpp
  src/rosbot/IMU.cpp
  src/rosbot/arduino_mpu9250_clk.cpp
  src/rosbot/arduino_mpu9250_i2c.cpp 
  src/rosbot/arduino_mpu9250_log.cpp 
  src/rosbot/inv_mpu.cpp
  src/rosbot/inv_mpu_dmp_motion_driver.cpp
  src/rosbot/SparkFunMPU9250-DMP.cpp
  src/VL53L0X.cpp
  )
endif()

set(PORT_CUSTOM_TARGETS)

include("${HFRAMEWORK_PATH}/ports/${PORT}/compilation.cmake")

add_library(hFramework ${HFRAMEWORK_FILES} ${PORT_FILES})
set_target_properties(hFramework PROPERTIES LINKER_LANGUAGE CXX)
if (PORT_CUSTOM_TARGETS)
  add_dependencies(hFramework ${PORT_CUSTOM_TARGETS})
endif()