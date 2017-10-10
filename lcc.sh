#!/bin/bash

dir=false
allim=false
cmd="./lcc.sh "

if (($# < 1)) ; then
  echo "The argument must be given"
  exit 0
fi

for arg in "$@"; do
  case $arg in
    -d)
      dir=true 
      ;;
    --allim) 
      allim=true 
      cmd+="--allim " 
      ;;
    *)
      if $dir; then
        if ! [ -d "$arg" ] ; then
          echo "The argument is not a directory"
          exit 0
        fi
        dir=false
        for lyapas_file in $(find $arg -name "*.lyapas"); do
          $cmd $lyapas_file                   
        done
      elif [[ $arg == *.lyapas ]]; then 
        name=${arg/.lyapas/}
        if $allim; then
          cat $arg | ./build/lyapas_im0 > "$name.im0"
          cat "$name.im0" | ./build/im0_im1 > "$name.im1"
          cat "$name.im1" | ./build/im1_im2 > "$name.im2"
          cat "$name.im2" | ./build/im2_im3 > "$name.im3"
        else
          name+=".im3"
          cat $arg \
            | ./build/lyapas_im0 \
            | ./build/im0_im1 \
            | ./build/im1_im2 \
            | ./build/im2_im3 > $name
        fi
        TXTCOLOR_DEFAULT="\033[0;m"
        TXTCOLOR_GREEN="\033[0;32m"
        echo -e $TXTCOLOR_GREEN"Processed $arg"$TXTCOLOR_DEFAULT
      else
        echo "Incorrect argument value $arg"
        exit 0
      fi
      ;;
  esac
done
