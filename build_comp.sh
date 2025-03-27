#!/bin/sh

if [ $# -ne 1 ]; then
  echo "Usage: $0 <component>"
  exit 1
fi

COMP=$1
mkdir -p "build/$COMP"

for f in "$COMP"/*.c; do
  echo "Compiling $f"
  gcc -o "build/$COMP/$(basename "$f").o" -c "$f"
done

echo "Creating static library lib$COMP.a"
ar rcs "build/lib$COMP.a" "build/$COMP"/*.o
