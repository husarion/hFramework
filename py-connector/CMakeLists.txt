cmake_minimum_required(VERSION 2.6)
get_filename_component(CURRENT_PY_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

project(hPyFramework NONE)

if(NOT HFRAMEWORK_PATH)
	set(HFRAMEWORK_PATH ${CURRENT_PY_DIR}/../)
endif()

if(NOT HPYTHON_PATH)
	set(HPYTHON_PATH ${CURRENT_PY_DIR}/../../hPython/)
endif()

include(${HFRAMEWORK_PATH}/robocore_common.cmake)
include(${HPYTHON_PATH}/hPython.cmake)

if(NOT RELEASE)
	set(TARGET_NAME hPyFrameworkd)
else()
	set(TARGET_NAME hPyFramework)
endif()

set(HPYFRAMEWORK_SOURCES
	${CMAKE_CURRENT_BINARY_DIR}/gen_hPyFramework.h
	${CMAKE_CURRENT_BINARY_DIR}/gen_hPyFramework.c
	${CMAKE_CURRENT_BINARY_DIR}/gen_hPyFramework.cpp)

set(EXPORTS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/exports)

add_custom_command(OUTPUT ${HPYFRAMEWORK_SOURCES}
	COMMAND python2.7 ${CURRENT_PY_DIR}/mpy-gen/generate.py --config ${EXPORTS_FILE} --outdir ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS ${EXPORTS_FILE})

add_library(${TARGET_NAME} bindir ${HPYFRAMEWORK_SOURCES})

set(BINDIR "${HFRAMEWORK_PATH}/libs/RoboCORE_${ROBOCORE_VERSION}_${ROBOCORE_TYPE}")
add_custom_command(OUTPUT bindir COMMAND mkdir -p ${BINDIR})
set_target_properties(${TARGET_NAME}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${BINDIR})
