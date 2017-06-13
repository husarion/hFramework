get_filename_component(PORT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

include_directories("${CURRENT_DIR}/third-party/esp-idf/components/freertos/include/")
include_directories("${CURRENT_DIR}/ports/esp32/build-sdk/build/include")
include_directories("${CURRENT_DIR}/third-party/esp-idf/components/esp32/include")
include_directories("${CURRENT_DIR}/third-party/esp-idf/components/tcpip_adapter/include")
include_directories("${CURRENT_DIR}/third-party/esp-idf/components/lwip/include/lwip")
include_directories("${CURRENT_DIR}/third-party/esp-idf/components/lwip/include/lwip/port")
include_directories("${CURRENT_DIR}/third-party/esp-idf/components/driver/include")
include_directories("${CURRENT_DIR}/third-party/esp-idf/components/nvs_flash/include")
include_directories("${CURRENT_DIR}/third-party/esp-idf/components/openssl/include")
include_directories("${CURRENT_DIR}/third-party/esp-idf/components/app_update/include")
include_directories("${CURRENT_DIR}/third-party/esp-idf/components/spi_flash/include")
include_directories("${CURRENT_DIR}/third-party/esp-idf/components/soc/esp32/include")

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
  COMMAND ${PORT_DIR}/collect_libs.sh ${PORT_DIR}/build-sdk ${CMAKE_BINARY_DIR} ${CURRENT_DIR}/third-party/esp-idf)

set(PORT_CUSTOM_TARGETS collect_libs)
