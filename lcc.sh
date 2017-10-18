#!/bin/bash

dir=false
allim=false
program_to_run=("lyapas_to_json.py" "lyapas_im0" "im0_im1" "im1_im2" "im2_im3")
path_to_im="`readlink -e ${0/lcc.sh/}`/build/"
path_to_json_formatter="`readlink -e ${0/lcc.sh/}`/"

if (($# < 1)) ; then
  echo "The argument must be given"
  exit 1
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
          exit 1
        fi
        dir=false
        for lyapas_file in $(find $arg -name "*.lyapas"); do
          $0 $lyapas_file
        done
      elif [[ $arg == *.lyapas ]]; then 
        name=${arg/.lyapas/}
        if $allim; then
          cat $arg | "${path_to_json_formatter}${program_to_run[0]}"> "$name.json"
          cat "$name.json" | "${path_to_im}${program_to_run[1]}" > "$name.im0"
          cat "$name.im0" | "${path_to_im}${program_to_run[2]}" > "$name.im1"
          cat "$name.im1" | "${path_to_im}${program_to_run[3]}" > "$name.im2"
          cat "$name.im2" | "${path_to_im}${program_to_run[4]}" > "$name.im3"
        else
          name+=".im3"
          cat $arg \
            | "${path_to_json_formatter}${program_to_run[0]}" \
            | "${path_to_im}${program_to_run[1]}" \
            | "${path_to_im}${program_to_run[2]}" \
            | "${path_to_im}${program_to_run[3]}" \
            | "${path_to_im}${program_to_run[4]}" > $name
        fi
        TXTCOLOR_DEFAULT="\033[0;m"
        TXTCOLOR_GREEN="\033[0;32m"
        echo -e $TXTCOLOR_GREEN"Processed $arg"$TXTCOLOR_DEFAULT
      else
        echo "Incorrect argument value $arg"
        exit 1
      fi
      ;;
  esac
done
