#!/bin/bash
set -e

format() {
    for src_file in $(find "$1" | egrep '\.(h|cpp)$'); do
        clang-format-5.0 -i -style=file $src_file;
    done
}


sources=("sources/im0_im1/" 
         "sources/im1_im2/" 
         "sources/im2_im3/"
         "sources/im3_im4/"
         "sources/im4_im5/"
         "sources/libs/translation_module/"
         "sources/libs/shared_utils/")

for i in ${sources[*]}; do 
    format $i
done

echo "Done"
