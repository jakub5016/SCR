#!/bin/bash 

for item in $(cat $1)
do 
    echo "Mail wysłany z skryptu" | mail -s "temat" "$item"
done