cmake_minimum_required(VERSION 2.6)
get_filename_component(CURRENT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

project(examples C CXX ASM)

if(NOT HFRAMEWORK_PATH)
	set(HFRAMEWORK_PATH "${CURRENT_DIR}/../")
endif()

include(${HFRAMEWORK_PATH}/hFramework.cmake)

file(GLOB examples ../examples/*.cpp)

foreach(ex ${examples})
	get_filename_component(name ${ex} NAME_WE)
	add_hexecutable(example_${name} ${ex})
endforeach()

add_hexecutable(sensor_gpio ../tests/sensor_gpio.cpp)
add_hexecutable(motors_single ../tests/motors_single.cpp)
