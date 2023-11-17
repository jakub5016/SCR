#!/bin/bash

for item in $@
do
    if [ -f ./$item ]; then
        echo $item
    elif [ -d $item ]; then
        dir_content=$(ls ./$item)
        if [[ $dir_content == "" ]];then
            echo "Katalog $item jest pusty"
        else
            echo "$dir_content"
        fi
    else
        echo "Nie ma takiego pliku ani folderu jak \"$item\""
    fi
done    