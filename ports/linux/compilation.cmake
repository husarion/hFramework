get_filename_component(PORT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

set(HFRAMEWORK_FILES
  )

set(PORT_FILES
  ${PORT_DIR}/src/init.cpp

  ${PORT_DIR}/src/hSystem.cpp
  ${PORT_DIR}/src/hSerial.cpp
  ${PORT_DIR}/src/hGPIO.cpp
  ${PORT_DIR}/src/hStorage.cpp
  ${PORT_DIR}/src/hGPIO.cpp
  ${PORT_DIR}/src/hWifi.cpp
  ${PORT_DIR}/src/board.cpp

  ${PORT_DIR}/src/hMutex.cpp
  ${PORT_DIR}/src/hCondVar.cpp
  src/hNetwork_bsd.cpp
  src/Other/hPrintfDev.cpp
  src/Other/hStreamDev.cpp
  src/Other/hPrintf.cpp
  src/Other/myprintf.cpp
  src/hSSL.cpp)
