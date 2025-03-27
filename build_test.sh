#!/bin/sh

./build_comp.sh core

gcc -lm -o build/test test.c build/libcore.a
