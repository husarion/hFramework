cmake_minimum_required(VERSION 2.8)
project(ARoboCoreProject NONE)

include(${HFRAMEWORK_PATH}/hFramework.cmake)
enable_module(hCloudClient)
enable_module(hSensors)
enable_module(hModules)
enable_module(hROS)

add_hexecutable(myproject main.cpp)
