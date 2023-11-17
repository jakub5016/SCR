#!/bin/bash


#  1 - nazwa katalogu
#  2 - rozszerzenie 
#  3 - ile dni 
#  4 - nazwa archiwum

if (( $# == 4 )); then
    files=$(find $1 -type f -mtime -"$3" -name "*.$2")
    tar -cvf "$4.tar" $files
else
    echo "Za malo parametrow"
fi