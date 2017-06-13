#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd $DIR/..

echo """
#ifndef __YCM_H__
#define __YCM_H__

#ifdef YCM
""" > include/ycm.h

for file in $(find ports/stm32/ src/ -name "*.cpp" -or -name "*.c"); do
	echo "#include \"../$file\""
	echo "#include \"../$file\"" >> include/ycm.h
done

echo """
#endif
#endif
""" >> include/ycm.h
