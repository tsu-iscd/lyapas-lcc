#!/bin/bash

set -e

if [ "$#" -ne 1 ]; then
    echo "Usage: ./compile_and_run.sh programm.lyapas"
    exit 1
fi

lyapas_dir="$(dirname $(readlink -e $0))"
lcc="${lyapas_dir}/lcc.sh"
printer="${lyapas_dir}/im_program_printer.py"

# имя без суффикса
programm_name="$(basename $1)"
programm_name="${programm_name%.*}"

programm_dir="$(dirname $(readlink -e $1))"

programm_lyapas="$(readlink -e $1)"
programm_im8="${programm_dir}/${programm_name}.im8"
programm_asm="${programm_name}.asm"

tmp_dir=$(mktemp -d)
pushd ${tmp_dir}
${lcc} ${programm_lyapas}
cat ${programm_im8} | ${printer} > ${programm_asm}

echo "--------------ASM LISTING----------------"
cat ${programm_asm}
echo "-----------------------------------------"
echo ""

echo "---------------COMPILE-------------------"
nasm -f elf64 -o "${programm_name}.o" ${programm_asm}
ld -o ${programm_name} "${programm_name}.o"
echo "-----------------------------------------"
echo ""

echo "------------------RUN--------------------"
./${programm_name}
echo "-----------------------------------------"
echo ""

popd
rm -rf ${tmp_dir}
