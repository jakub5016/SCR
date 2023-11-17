#!/bin/bash

if [[ "$1" == '-a' ]]; then
    x=$(ls -a | wc -l)
else
    x=$(ls | wc -l)
fi

echo $x