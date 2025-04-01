#!/bin/sh

if [ "$1" = "d" ]; then
  CFLAGS="$CFLAGS -g"
fi

gcc $CFLAGS -lm -o build/e1p e1p/main.c build/libcore.a
