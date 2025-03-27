#!/bin/sh

if [ "$1" = "d" ]; then
  CFLAGS="$CFLAGS -g"
fi

gcc $CFLAGS -lm -o build/test test.c build/libcore.a
