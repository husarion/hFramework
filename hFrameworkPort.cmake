set(CMAKE_SYSTEM_NAME "Generic")
get_filename_component(HFRAMEWORK_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3
cmake_policy(SET CMP0015 NEW) # absolute link_directories
cmake_policy(SET CMP0011 OLD) # policy

#set(CMAKE_ASM_COMPILER_WORKS 1)
#set(CMAKE_ASM_ABI_COMPILED TRUE)
#set(CMAKE_ASM_COMPILER_ID GNU)
#set(CMAKE_ASM_COMPILER_ID_RUN 1)

set(CMAKE_STATIC_LIBRARY_SUFFIX_CXX ".a")
set(CMAKE_STATIC_LIBRARY_SUFFIX_C ".a")
set(CMAKE_STATIC_LIBRARY_PREFIX_CXX "lib")
set(CMAKE_STATIC_LIBRARY_PREFIX_C "lib")

macro(disable_compiler_detection)
  set(CMAKE_C_COMPILER ${prefix}gcc${exesuffix})
  set(CMAKE_ASM_COMPILER ${prefix}gcc${exesuffix})
  set(CMAKE_CXX_COMPILER ${prefix}g++${exesuffix})

  set(CMAKE_CROSSCOMPILING true)

  set(CMAKE_C_COMPILER_ID GNU)
  set(CMAKE_C_COMPILER_WORKS 1)
  set(CMAKE_C_ABI_COMPILED TRUE)
  set(CMAKE_C_COMPILER_ID GNU)
  set(CMAKE_C_COMPILER_ID_RUN 1)

  set(CMAKE_CXX_COMPILER_ID GNU)
  set(CMAKE_CXX_COMPILER_WORKS 1)
  set(CMAKE_CXX_ABI_COMPILED TRUE)
  set(CMAKE_CXX_COMPILER_ID GNU)
  set(CMAKE_CXX_COMPILER_ID_RUN 1)

  enable_language(C CXX ASM)

  set(CMAKE_AR ${prefix}ar${exesuffix}) # needs to be after enable_language
  set(CMAKE_RANLIB ${prefix}ranlib${exesuffix})
endmacro()

option(DEBUG "Enable debug informations" OFF)

# if(NOT BOARD_TYPE)
	# set(BOARD_TYPE "big")
# else()
	# if(BOARD_TYPE EQUAL 1)
		# set(BOARD_TYPE "mini")
	# elseif(BOARD_TYPE EQUAL 2)
		# set(BOARD_TYPE "big")
	# endif()
# endif()

if(NOT DEBUG)
	if(NOT RELEASE)
		set(RELEASE 1)
	endif()
else()
	if(RELEASE)
		set(RELEASE 0)
	endif()
endif()

if(NOT DEFINED BOARD_TYPE)
  message("BOARD_TYPE missing")
endif()

string(TOLOWER ${BOARD_TYPE} BOARD_TYPE)

set(NEED_BOARD true)

if(NOT BOARD_VERSION)
  set(BOARD_VERSION 1.0.0)
endif()

if(BOARD_TYPE STREQUAL "robocore")
  add_definitions(-DBOARD_TYPE=2)
  set(PORT stm32)
elseif(BOARD_TYPE STREQUAL "core2")
  add_definitions(-DBOARD_TYPE=3)
  set(PORT stm32)
elseif(BOARD_TYPE STREQUAL "core2mini")
  add_definitions(-DBOARD_TYPE=4)
  set(PORT stm32)
elseif(BOARD_TYPE STREQUAL "raspberrypi")
  set(PORT linux)
  set(ARCH arm)
endif()

string(REPLACE "." "_" BOARD_VERSION ${BOARD_VERSION})

string(REPLACE "_" ";" VERSION_LIST ${BOARD_VERSION})
list(GET VERSION_LIST 0 BOARD_VERSION_A)
list(GET VERSION_LIST 1 BOARD_VERSION_B)
list(GET VERSION_LIST 2 BOARD_VERSION_C)

add_definitions(-DPORT=${PORT})
add_definitions(-DBOARD_VERSION=${BOARD_VERSION})
add_definitions(-DBOARD_VERSION_A=${BOARD_VERSION_A})
add_definitions(-DBOARD_VERSION_B=${BOARD_VERSION_B})
add_definitions(-DBOARD_VERSION_C=${BOARD_VERSION_C})

if(EXISTS ${HFRAMEWORK_DIR}/hFrameworkInternal.cmake)
  message("-- Using hFramework from workspace")
  set(USES_SDK false)
else()
  set(USES_SDK true)
endif()

include("${HFRAMEWORK_DIR}/ports/${PORT}/options.cmake")

#set(CMAKE_FIND_ROOT_PATH toolchain)
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS "s;S")

add_definitions(-DSUPPORT_CPLUSPLUS)

set(errors_flags "-Wall -Wextra -Werror=parentheses -Werror=implicit -Wno-write-strings -Wno-main -Wno-unused-parameter")
set(errors_flags_cpp "-Wno-reorder")
if (${PORT} STREQUAL esp32)
  set(optimization_flags "-Og")
else()
  set(optimization_flags "-O0")
endif()

# set(common_flags "-static ${errors_flags} -ffunction-sections -fdata-sections")
set(common_flags "${errors_flags} -g -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti") # -Wfatal-errors
set(common_lflags "-fno-exceptions -fno-rtti")

if(NOT RELEASE)
	add_definitions(-DDEBUG)
	set(common_flags "${common_flags} -g")
endif()

macro(optimization opt)
	set(optimization_flags "-O${opt}")
	update_flags()
endmacro()

macro(update_flags)
  set(tmp "${common_flags} ${optimization_flags} ${port_flags}")

  set(CMAKE_EXE_LINKER_FLAGS "${compiler_flags} ${tmp} -Wl,--gc-sections -lg -lstdc++ -lsupc++")
  set(CMAKE_CXX_FLAGS        "${compiler_flags} ${tmp} ${errors_flags_cpp} -std=c++11")
  set(CMAKE_C_FLAGS          "${compiler_flags} ${tmp} -std=c99")
  set(CMAKE_ASM_FLAGS        "${tmp}")
  set(CMAKE_CXX_LINK_EXECUTABLE "${prefix}g++ ${CMAKE_EXE_LINKER_FLAGS} <OBJECTS> -o <TARGET> ${ADDITIONAL_LINK_DIRS} ${ADDITIONAL_LIBS} ${port_lflags} ${common_lflags}")
endmacro()

update_flags()

include_directories("${HFRAMEWORK_DIR}/include/")
