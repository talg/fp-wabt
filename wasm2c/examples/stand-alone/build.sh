#!/usr/bin/bash
if [ -z "$1" ]
  then
    echo "Compiles wasm file into standalone binary"
    echo "usage: build.sh foo.wasm"
    echo "outputs: foo.elf"
    exit
fi
set -x
ORIGIN_DIR=$(pwd)
SCRIPT_DIR=$(cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

cd $SCRIPT_DIR
make clean

cp ${ORIGIN_DIR}/$1 ./input.wasm
make input.c

if [ -n "${NOBOUND}" ]; then
       ./remove-base.sh input.c
fi

fn_out=$(basename -- "$1")
fn_out="${fn_out%%.*}.elf"

rm -f ./${fn_out}
make input.elf
cp input.elf ${ORIGIN_DIR}/${fn_out}
