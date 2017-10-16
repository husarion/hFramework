cmake_minimum_required(VERSION 2.8)
project(ARoboCoreProject NONE)

include(${HFRAMEWORK_PATH}/hFramework.cmake)
#include(C:/Users/werewolf/Documents/GitHub/hFramework/hFramework.cmake)
#enable_module(hCloudClient)
#enable_module(hSensors)
#enable_module(hModules)
#enable_module(hROS)

add_hexecutable(myproject main.cpp SparkFunMPU9250-DMP.cpp util/inv_mpu.cpp util/inv_mpu_dmp_motion_driver.cpp util/arduino_mpu9250_log.cpp util/arduino_mpu9250_i2c.cpp util/arduino_mpu9250_clk.cpp)