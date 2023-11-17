#!/bin/bash

echo "Moje PID to:" $$

was_presed=false
start=$(date +%s)


function SIGUSR1_handler() {
    if [ $was_presed = false ]; then
        start=$(date +%s)
        was_presed=true
        echo "Czy na pewno chchesz wyłączyć?"
    elif [ $(( $(date +%s) - $start < 5 )) ]; then
        exit
    else
        was_presed=false
    fi
}


while true; do
trap '' SIGFPE 
trap "echo WINDOWS CHANGE - redraw " SIGWINCH 
trap SIGUSR1_handler SIGUSR1
done   