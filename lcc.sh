#!/bin/bash

dir=false
allim=false
program_to_run=("lyapas_to_json.py" "lyapas_im0" "im0_im1" "im1_im2" "im2_im3")
path_to_im="`readlink -e ${0/lcc.sh/}`/build/"
path_to_json_formatter="`readlink -e ${0/lcc.sh/}`/"

function translate()
{
  name=${1/.lyapas/}
  if $allim; then
    cat $1 | "${path_to_json_formatter}${program_to_run[0]}"> "$name.json"
    cat "$name.json" | "${path_to_im}${program_to_run[1]}" > "$name.im0"
    cat "$name.im0" | "${path_to_im}${program_to_run[2]}" > "$name.im1"
    cat "$name.im1" | "${path_to_im}${program_to_run[3]}" > "$name.im2"
    cat "$name.im2" | "${path_to_im}${program_to_run[4]}" > "$name.im3"
  else
    name+=".im3"
    cat $1 \
      | "${path_to_json_formatter}${program_to_run[0]}" \
      | "${path_to_im}${program_to_run[1]}" \
      | "${path_to_im}${program_to_run[2]}" \
      | "${path_to_im}${program_to_run[3]}" \
      | "${path_to_im}${program_to_run[4]}" > $name
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
      cmd+=" --allim "
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
      elif [[ $arg == *.lyapas ]]; then 
        translate $arg
      else
        echo "Incorrect argument value \"$arg\""
        show_usage_and_exit
      fi
      ;;
  esac
done
