#!/bin/bash
cd "$(dirname "$0")"/..
# astyle matching rules are crazy, better match the files ourselves

astyle \
  --options=astylerc.cpp \
  $(find Libraries -name '*.c' | grep -Pv 'Libraries/hUSB/usb|Libraries/hSD/FatFS/ff.c') \
  $(find Libraries -name '*.h' | grep -Pv 'Libraries/hUSB/usb|Libraries/hSD/FatFS/FATFS_include/ff.h') \
  $(find Libraries -name '*.cpp') \
  examples/*.cpp \
  include/*.h

devtools/fix_docs.sh include/*.h
