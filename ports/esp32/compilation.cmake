get_filename_component(PORT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

set(HFRAMEWORK_FILES
  )

set(PORT_FILES
  ${PORT_DIR}/src/main.cpp
  ${PORT_DIR}/src/hSerial.cpp
  ${PORT_DIR}/src/hSystem.cpp
  ${PORT_DIR}/src/hGPIO.cpp
  ${PORT_DIR}/src/hWifi.cpp
  ${PORT_DIR}/src/ESPStorage.cpp
  ${PORT_DIR}/src/OTA.cpp

  src/hSSL.cpp

  src/hNetwork_bsd.cpp
  src/hMutex_rtos.cpp
  src/hSystem_rtos.cpp
  src/hGenericQueue_rtos.cpp
  src/hCondVar.cpp
  src/Other/hStreamDev.cpp
  src/Other/hPrintf.cpp
  src/Other/hPrintfDev.cpp
  src/Other/myprintf.cpp)

add_custom_target(
  collect_libs
  USES_TERMINAL
  COMMAND env IDF_PATH="${IDF_PATH}" ${PORT_DIR}/collect_libs.sh ${PORT_DIR}/build-sdk ${CMAKE_BINARY_DIR} ${CURRENT_DIR}/third-party/esp-idf)

set(PORT_CUSTOM_TARGETS collect_libs)
