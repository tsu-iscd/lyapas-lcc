#!/bin/bash

set -e
set -o pipefail

dir=false
allim=false
path_to_lyapas_dir="$(dirname $(readlink -e $0))"
program_to_run=($path_to_lyapas_dir"/lyapas_to_json.py" 
                $path_to_lyapas_dir"/build/lyapas_im0" 
                $path_to_lyapas_dir"/build/im0_im1" 
                $path_to_lyapas_dir"/build/im1_im2" 
                $path_to_lyapas_dir"/build/im2_im3"
                $path_to_lyapas_dir"/build/im4_im5"
                $path_to_lyapas_dir"/build/im3_im4"
                $path_to_lyapas_dir"/build/im7_im8")
suffix=("" ".json" ".im0" ".im1" ".im2" ".im3" ".im4" ".im5" ".im8")

function translate()
{
  name=${1%.*}
  if [[ "$1" == *.* ]]; then
    suffix[0]=".${1##*.}"
  fi
  if $allim; then
    for i in ${!program_to_run[@]}; do
        cat "$name${suffix[$i]}" | ${program_to_run[$i]} > "$name${suffix[$i + 1]}"
    done
  else
    name+=${suffix[-1]}
    cat $1 \
      | "${program_to_run[0]}" \
      | "${program_to_run[1]}" \
      | "${program_to_run[2]}" \
      | "${program_to_run[3]}" \
      | "${program_to_run[4]}" \
      | "${program_to_run[5]}" \
      | "${program_to_run[6]}" \
      | "${program_to_run[7]}" > $name
  fi
  TXTCOLOR_DEFAULT="\033[0;m"
  TXTCOLOR_GREEN="\033[0;32m"
    echo -e $TXTCOLOR_GREEN"Processed $1"$TXTCOLOR_DEFAULT
}

function show_usage_and_exit()
{
  echo "Usage: ./lcc.sh [--allim] FILE ..."
  echo -e "   or: ./lcc.sh [--allim] -d DIRECTORY ... \n"
  echo -e "Translate lyapas file to the latest intermediate language\n"
  echo " --allim: save the result of translation to all intermediate languages"
  exit 1
}

if (($# < 1)) ; then
  show_usage_and_exit
fi

for arg in "$@"; do
  case $arg in
    -d)
      dir=true 
      ;;
    --allim) 
      allim=true 
      ;;
    *)
      if $dir; then
        if ! [ -d "$arg" ] ; then
          echo "The argument is not a directory"
          show_usage_and_exit
        fi
        dir=false
        for lyapas_file in $(find $arg -name "*.lyapas"); do
          translate $lyapas_file
        done
      else
        translate $arg
      fi
      ;;
  esac
done
