cmake_minimum_required(VERSION 2.8)
project(ARoboCoreProject NONE)

include(${HFRAMEWORK_PATH}/hFramework.cmake)

add_hexecutable(myproject main.cpp)