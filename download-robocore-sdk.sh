#!/bin/bash
wget https://files.husarion.com/RoboCORE-devel.zip
mkdir robocore/
cd robocore/
unzip ../RoboCORE-devel.zip
rm -r libs/stm32_core2*
rm -r tools/
rm *.cmake
rm -r project_template
rm ../RoboCORE-devel.zip
