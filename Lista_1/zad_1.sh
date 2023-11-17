#!/bin/bash

day_of_the_week=$(date +%a)

if [ $day_of_the_week == "Sat" ] || [ $day_of_the_week == "Sun" ]; then
    echo "Witaj w weekend, czas na melanż"
else 
    echo "Witaj w dniu roboczym"
fi
