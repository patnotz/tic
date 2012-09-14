#!/bin/sh

VS=$1
shift
HEADER_PATH=$1
shift
VH="$HEADER_PATH/tic_autoversion.hpp"

VERSION=$($VS)

if test -r $VH 
then
	PREVIOUS_VERSION=$(grep TIC_VERSION $VH | sed -e 's/^#define TIC_VERSION //' | sed -e 's/"//g')
else
	PREVIOUS_VERSION=unset
fi

if test "$VERSION" != "$PREVIOUS_VERSION"
then
	echo "#ifndef TIC_AUTOVERSION_HPP_" > $VH
	echo "#define TIC_AUTOVERSION_HPP_" >> $VH
	echo "" >> $VH
	echo "#define TIC_VERSION \"$VERSION\"" >> $VH
	echo "#endif // TIC_AUTOVERSION_HPP_" >> $VH
fi
