#!/bin/bash

BINDIR=$(cd "$(dirname "$0")"; pwd)

# create a rudimentary leaf directory Makefile.am
# $1 = directory to create a leaf makefile in

if [ `find $1 -type d | wc -l` == 1 ] ; then
	echo "Leaf dir $1"

	CFILES=`cd $1 ; echo *.c`
	DIRNAME=`basename $1`

	cat "$BINDIR/Makefile-leaf.template" | sed "s/%DIRNAME%/$DIRNAME/g" | sed "s/%CFILES%/$CFILES/g" > $1/Makefile.am
else
	echo "Branch dir $1"
	DIRS=""
	for I in `ls $1` ; do
		if [ -d "$1/$I" ] ; then
			if [ "$DIRS" == "" ] ; then
				DIRS="$I"
			else
				DIRS="$DIRS $I"
			fi
		fi
	done

	echo "SUBDIRS=$DIRS" > $1/Makefile.am
	echo "ACLOCAL_AMFLAGS=-I m4" >> $1/Makefile.am

        CFILES=`cd $1 ; echo *.c`

	if [ "$CFILES" != "*.c" ] ; then
		echo "With source code"

	        DIRNAME=`basename $1`
	        cat "$BINDIR/Makefile-leaf.template" | sed "s/%DIRNAME%/$DIRNAME/g" | sed "s/%CFILES%/$CFILES/g" >> $1/Makefile.am
	else
		echo "No source code"
	fi
fi

# end


