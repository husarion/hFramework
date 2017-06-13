get_filename_component(HFRAMEWORK_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3
set(HFRAMEWORK_DIR_Q "\"${HFRAMEWORK_DIR}\"")

include("${HFRAMEWORK_DIR}/hFrameworkPort.cmake")

set(compiler_flags "-g")

macro(add_component_lib name)
  enable_module(${name})
endmacro()

macro(enable_module name)
  if (USES_SDK)
    include_directories("${HFRAMEWORK_DIR}/include/${name}")
    set(module_libraries "${module_libraries} -l${name}")
  else()
    execute_process(
      COMMAND "${HFRAMEWORK_PATH}/../hcommon/hdev-findmodule" ${name}
      OUTPUT_VARIABLE module_path
      RESULT_VARIABLE err)
    if(${err})
      message(FATAL_ERROR "cannot find module")
    endif()
    string(STRIP ${module_path} module_path)

    message("-- Using module from workspace: ${module_path}")
    include_directories("${module_path}/include/")
    set(ADDITIONAL_LINK_DIRS "${ADDITIONAL_LINK_DIRS} -L${module_path}/build/${PORT}_${BOARD_TYPE}_${BOARD_VERSION_DOT}")
    set(module_libraries "${module_libraries} -l${name}")
  endif()
endmacro()

if(WIN32)
  set(TOOLS_ARCH_NAME win)
else()
  set(TOOLS_ARCH_NAME amd64-linux)
endif()

macro(add_hexecutable name)
  if (NOT ${PORT} STREQUAL esp32)
    list(APPEND ADDITIONAL_LIBS "${module_libraries} -lhFramework")
  endif()
  update_flags()
  add_executable("${name}.elf" ${ARGN})

  add_hexecutable_port(${name})
  set_target_properties("${name}.elf" PROPERTIES
    LINKER_LANGUAGE CXX
    SUFFIX "")

  if(NOT DEFINED ${main_executable})
    set(main_executable ${name})
    add_custom_target("flash"
      DEPENDS "flash_${name}")

    printvars_target_done()
  endif()
endmacro()

### Support for printvars

get_property(include_spaces DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
set(vars_info "")

foreach(v IN LISTS include_spaces)
  set(vars_info "${vars_info}::include=${v}")
endforeach(v)

function(printvars_target_done)
  add_custom_target(
    printvars
    COMMAND
    cmake -E echo 'VARS${vars_info}::sdk=${HFRAMEWORK_PATH}::main_executable=${main_executable}::none=')
endfunction()

### End printvars

include_directories("${HFRAMEWORK_DIR}/include")

if (USES_SDK)
  set(ADDITIONAL_LINK_DIRS "-L${HFRAMEWORK_DIR_Q}/libs/${PORT}_${BOARD_TYPE}_${BOARD_VERSION}")
else()
  string(REPLACE _ . BOARD_VERSION_DOT ${BOARD_VERSION})
  set(ADDITIONAL_LINK_DIRS "-L${HFRAMEWORK_DIR_Q}/build/${PORT}_${BOARD_TYPE}_${BOARD_VERSION_DOT}")
endif()

update_flags()

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
