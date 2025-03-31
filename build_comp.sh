#!/bin/sh

if [ $# -lt 1 ]; then
  echo "Usage: $0 <component>"
  exit 1
fi

if [ $2 = "d" ]; then
  CFLAGS="$CFLAGS -g"
fi

COMP=$1
mkdir -p "build/$COMP"

for f in "$COMP"/*.c; do
  echo "Compiling $f"
  gcc $CFLAGS -std=c11 -o "build/$COMP/$(basename "$f").o" -c "$f"
done

echo "Creating static library lib$COMP.a"
ar rcs "build/lib$COMP.a" "build/$COMP"/*.o
