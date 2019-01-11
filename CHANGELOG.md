# Change Log
All notable changes to the hFramework library.

## [Unreleased]
- Fixed ADC ports assignment for CORE2mini
## [1.5.16] - 2018-10-22
- Corrected timer association for encoders
- Added pinout for CORE2mini 1.0.0 and swapped master/slave motor port
## [1.5.15] - 2018-10-17
- Removed calls to pin 91
- Fixed wheel reset
## [1.5.13] - 2018-08-27
- Updated documentation
- Refactored wheel.h
- Updated ROSbot parameters
## [1.5.12] - 2018-08-09
- Refactored code for better IntelliSense support on windows
- Added changelog
## [1.5.11] - 2018-07-30
- Added enum for selecting IMU type
- Moved sensors reading to separate tasks
## [1.5.10] - 2018-07-25
- Added ROSbot support class for odometry, velocity command, battery, laser and infrared range sensors and IMU
- Added method for switching the 5V rail for hExt and hSensors
- Added new examples
- Fixed typos in hMotor braking modes
- Small fixes
## [1.5.8] - 2018-05-14
- Added description for getTaskList(), getStats() in hSystem class
- Added build improvements to ESP32 port
- Updated esp-idf version
- Fix: swapped pins in hMotA and hMotB electrical interfaces
- Fixed bugs in I2C interface (mostly with rxDelay in I2C rw function)
## [1.5.7] - 2018-01-10
- Fixed Arduino library compilation errors
- Fixed getRefTime() method (added critical section)
## [1.5.6] - 2017-12-11
- Added Arduino support for hFramework
- Fixed default baudrate for hSerial
## [1.5.5] - 2017-10-19
- getRefTime() now returns 64-bit value instead of 32-bit
## [1.5.2] - 2017-09-18
- Husarion extension for VSCode update only
## [1.4.8] - 2017-08-30
- Added Wi-Fi scanning for Linux
- Updated examples and fixed errors
- Updated API documentation
- Changed default RPi serial port baudrate to 500000
- Improved compatibility with VSCode
- Fixed malloc to be thread safe
- Fixed building for STM32
- Fixed spurious wakeup and getRefTime on Linux port
- Fixed delaySync() function in hSystem
- Fixed fault logs
## [1.4.6] - 2017-06-28
- Added MIT license note
- Updated IDF version for ESP32
- Fixed network scanning for ESP32
## [1.4.5] - 2017-04-27
- Husarion extension for VSCode update only
## [1.4.4] - 2017-02-21
- The first official release that adds support for Visual Studio Code and ROS
- Added support for build on Windows
- Enabled debug info by default
- Restored Linux port
- Removed legacy files
- crosslib is no longer used
- Fixed likner error in non-SDK build
- Fixed ADC measurement coefficient
## [1.3.10] - 2016-10-20
- Updated licenses
## [1.3.5] - 2016-10-07
- Fixed API for SPI
## [1.3.3] - 2016-09-28
- Fixed hButton waitForPressed and waitForReleased methods
## [1.2.15] - 2016-09-16
- Added pin definitions for CORE2mini v0.2
- Fixed CORE2mini compilation
- Fixed USB support
- Updated pinout
## [1.2.12] - 2016-09-13
- Updated voltage divider values
## [1.2.10] - 2016-09-12
- Added CORE2 v1.0.0 support
- Fixed cmake
- Updated flasher to use with CORE2 and changed name from robocore-flasher to core2-flasher
## [1.2.6] - 2016-09-05
- Changed LEDs order to match the new CORE2 PCB
## [1.2.3] - 2016-09-04
- Added hGenericQueue
- Fixed examples
- Fixed cmake scripts
## [1.2.2] - 2016-07-05
- Added support for CORE2 v0.2
- Added initial version of CORE2mini port
- Added hLegoSensor classes
- Added RPi serial port support
- Added hServoModule
- Changed the way of defining examples
- Changed polarity enum to C++11 enum class
- Udpated API description
- Removed USB for CORE2
- Fixed build script
- Fixed PWM support
- Fixed baudrate methods in Linux port
- Fixed myEncoder module
- Fixed STM32 and Linux ports
## [1.1.4] - 2016-02-11
- Changed @ to __ as radio group/button id separator
## [1.1.3] - 2016-02-11
- Added hQuaternion and hVector3D 
- Added MPU9250 DMP support
- Increased hMain thread stack size to 1000B 
- Fixed hMotor::calibrate method
## [1.1.0] - 2016-02-05
- Added UiComboBox and UiRadioButton/Group
- Changed KeyCodes define to KeyCode enum class 
## [1.0.19] - 2016-01-21
- Added example for microSD card
## [1.0.18] - 2016-01-13
- Fixed printing negative float values
## [1.0.17] - 2016-01-13
- Changed hLocalSerial::dataAvailable() and hCloudClient::dataAvailable() to hLocalSerial::available() and hCloudClient::available()
## [1.0.16] - 2015-12-29
- Fixed bootloaders
## [1.0.14] - 2015-12-29
- Added versioning to the software bootloader 
- Added printing the software bootloader version to robocore-flasher
## [1.0.11] - 2015-11-30
- Added active/passive braking for motor control
## [1.0.10] - 2015-11-30
- Added examples for DistanceSensor and MPU9250 modules
## [1.0.9] - 2015-10-23
- Added support for RoboCORE 1.0.0
## [1.0.8] - 2015-10-21
- Renamed ButtonEventType::PRESSED to ButtonEventType::Pressed and ButtonEventType::RELEASED to ButtonEventType::Released, the same for KeyEventType
## [1.0.5] - 2015-09-19
- Added custom implementation of [vsn]printf functions 
- Added hPrintfContext class 
- Added readAll method to hStreamDev interface 
- Added set method to hLED class 
- Implemented access to SPI and I2C on hExt ports 
- Each of hSPI, hI2C, hSerial peripheral can be switched to GPIO mode 
- Removed pin8 and pin9 from hExt (these pins are accessible from hSerial class) 
- sys.taskCreate now returns hTask object which can be used to join to thread 
- sys.taskCreate now uses std::function which allows to passing lambdas with context 
- Added method for printing thread status (sorted) 
- Added hElapsedTimer for easy handling recurring tasks 
- Added setCoeffs and reset methods to hPID class
## [1.0.2] - 2015-08-27
- Added sys.syslog, sys.vsyslog, sys.enableSyslog, sys.disableSyslog methods, internal log will go to Serial when syslog is enabled
## [1.0.1] - 2015-08-24
- Added hSemaphore class
## [1.0.0] - unknown
- First revision considered stable
