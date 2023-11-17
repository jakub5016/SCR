#!/bin/bash

PID=$1

case $2 in

    "SIGFPE")
        kill -s SIGFPE $1
        ;;

    "SIGWINCH")
        kill -s SIGWINCH $1
        ;;
    
    "SIGUSR1")
        kill -s SIGUSR1 $1
        ;;
    
    *)
    echo "Nieznany syganł"
    ;;

esac

# kill -s SIGFPE $1
# kill -s SIGWINCH $1
# kill -s SIGUSR1  $1