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
include_directories("${CURRENT_DIR}/robots/rosbot/include/")
include_directories("${CURRENT_DIR}/robots/rosbot/MPU9250lib")
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
  robots/rosbot/src/ROSbot.cpp
  robots/rosbot/src/wheel.cpp
  robots/rosbot/src/VL53L0X.cpp
  robots/rosbot/src/IMU.cpp
  robots/rosbot/MPU9250lib/arduino_mpu9250_clk.cpp
  robots/rosbot/MPU9250lib/arduino_mpu9250_i2c.cpp
  robots/rosbot/MPU9250lib/arduino_mpu9250_log.cpp
  robots/rosbot/MPU9250lib/inv_mpu_dmp_motion_driver.cpp
  robots/rosbot/MPU9250lib/inv_mpu.cpp
  robots/rosbot/MPU9250lib/SparkFunMPU9250-DMP.cpp
)

set(PORT_CUSTOM_TARGETS)

include("${HFRAMEWORK_PATH}/ports/${PORT}/compilation.cmake")

add_library(hFramework ${HFRAMEWORK_FILES} ${PORT_FILES})
set_target_properties(hFramework PROPERTIES LINKER_LANGUAGE CXX)
if (PORT_CUSTOM_TARGETS)
  add_dependencies(hFramework ${PORT_CUSTOM_TARGETS})
endif()