#!/bin/bash
set -e

format() {
    for src_file in $(find "$1" | egrep '\.(h|cpp)$'); do
        clang-format-5.0 -i -style=file $src_file;
    done
}


sources=("./") 

for i in ${sources[*]}; do 
    format $i
done

echo "Done"
