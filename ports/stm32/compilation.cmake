get_filename_component(PORT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

include_directories("${CURRENT_DIR}/third-party/FatFS/FATFS_include/")
include_directories("${CURRENT_DIR}/src/hPeriph/")
include_directories("${CURRENT_DIR}/third-party/FreeRTOS/include/")
include_directories("${CURRENT_DIR}/third-party/FreeRTOS/")
include_directories("${CURRENT_DIR}/third-party/FreeRTOS/portable/GCC/ARM_CM4F/")
include_directories("${CURRENT_DIR}/third-party/usblib/")
include_directories("${CURRENT_DIR}/src/hSystem/")
include_directories("${CURRENT_DIR}/src/Other/")
include_directories("${CURRENT_DIR}/third-party/eeprom/")
include_directories("${CURRENT_DIR}/third-party/cmsis_boot/")
include_directories("${CURRENT_DIR}/third-party/cmsis/")
include_directories("${CURRENT_DIR}/third-party/cmsis_lib/include/")

set(PORT_FILES
	${PORT_DIR}/src/init.cpp

	${PORT_DIR}/src/syscalls.c

	${PORT_DIR}/src/hLED.cpp
	${PORT_DIR}/src/hStorage.cpp
	${PORT_DIR}/src/hExt.cpp
	${PORT_DIR}/src/hCAN.cpp
	${PORT_DIR}/src/hSerial.cpp
	${PORT_DIR}/src/hSensor.cpp
	${PORT_DIR}/src/hLegoSensor.cpp
	${PORT_DIR}/src/hI2C.cpp
	${PORT_DIR}/src/hSPI.cpp
	${PORT_DIR}/src/hSD.cpp
	${PORT_DIR}/src/hGPIO.cpp
	${PORT_DIR}/src/hButton.cpp
	${PORT_DIR}/src/hServoModule.cpp
	${PORT_DIR}/src/system.cpp

	${PORT_DIR}/src/hPeriph/myADC.cpp
	${PORT_DIR}/src/hPeriph/myCAN.c
	# ${PORT_DIR}/src/hPeriph/myDAC.c
	${PORT_DIR}/src/hPeriph/myDMA.c
	${PORT_DIR}/src/hPeriph/myEncoder.c
	${PORT_DIR}/src/hPeriph/myEXTI.cpp
	${PORT_DIR}/src/hPeriph/myGPIO.c
	${PORT_DIR}/src/hPeriph/myI2C.c
	${PORT_DIR}/src/hPeriph/myPWM.c
	${PORT_DIR}/src/hPeriph/mySPI.c
	${PORT_DIR}/src/hPeriph/myTimer.c
	${PORT_DIR}/src/hPeriph/myUSART.c
	# ${PORT_DIR}/src/hPeriph/myUSART_dma.c
	${PORT_DIR}/src/hPeriph/myUSART_isr.c
	${PORT_DIR}/src/hPeriph/myUSART_poll.c

	${PORT_DIR}/src/runTimeStatsInit.c

	${PORT_DIR}/src/hMotor.cpp
	${PORT_DIR}/src/hMotorPimpl.cpp
	
	${PORT_DIR}/src/twi.cpp
	${PORT_DIR}/src/Wire.cpp

	src/hGenericQueue_rtos.cpp
	src/hMutex_rtos.cpp
	src/hSystem_rtos.cpp
        src/hCondVar.cpp

	third-party/cmsis_lib/source/misc.c
	third-party/FatFS/diskio.c
	third-party/FatFS/ff.c
	third-party/FatFS/spi.c
	third-party/FatFS/unicode.c

	third-party/cmsis_lib/source/stm32f4xx_adc.c
	third-party/cmsis_lib/source/stm32f4xx_can.c
	third-party/cmsis_lib/source/stm32f4xx_dma.c
	third-party/cmsis_lib/source/stm32f4xx_exti.c
	third-party/cmsis_lib/source/stm32f4xx_flash.c
	third-party/cmsis_lib/source/stm32f4xx_gpio.c
	third-party/cmsis_lib/source/stm32f4xx_i2c.c
	third-party/cmsis_lib/source/stm32f4xx_pwr.c
	third-party/cmsis_lib/source/stm32f4xx_rcc.c
	third-party/cmsis_lib/source/stm32f4xx_spi.c
	third-party/cmsis_lib/source/stm32f4xx_syscfg.c
	third-party/cmsis_lib/source/stm32f4xx_tim.c
	third-party/cmsis_lib/source/stm32f4xx_usart.c

	third-party/cmsis_boot/startup/startup_stm32f4xx.s
	third-party/cmsis_boot/system_stm32f4xx.c

	third-party/eeprom/eeprom.c

	third-party/FreeRTOS/croutine.c
	third-party/FreeRTOS/event_groups.c
	third-party/FreeRTOS/hooks.c
	third-party/FreeRTOS/list.c
	third-party/FreeRTOS/portable/GCC/ARM_CM4F/port.c
	third-party/FreeRTOS/queue.c
	third-party/FreeRTOS/tasks.c
	third-party/FreeRTOS/timers.c
)

if(BOARD_TYPE STREQUAL "robocore")
	list(APPEND PORT_FILES
		${PORT_DIR}/src/hUSB/hUSB.cpp
		${PORT_DIR}/src/hUSB/usb/usb_bsp.c
		${PORT_DIR}/src/hUSB/usb/usbh_adk_core.c
		${PORT_DIR}/src/hUSB/usb/usbh_usr.c

		third-party/usblib/usb_core.c
		third-party/usblib/usb_hcd.c
		third-party/usblib/usb_hcd_int.c
		third-party/usblib/usbh_core.c
		third-party/usblib/usbh_hcs.c
		third-party/usblib/usbh_ioreq.c
		third-party/usblib/usbh_stdreq.c
	)
endif()
