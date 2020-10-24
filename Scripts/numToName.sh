#!/bin/bash

while read command
do
    # if $command is a number then it goes into grep else it is printed
    if [ $skip=false ] && [ "$command" -eq "$command" ]  2>/dev/null; then
        grep -oh "\w*\s*\b$command\b" scanner.h 
        # if the command is something that can be followed by a string, then its followup is immidiatly printed so it doesnt go to grep
        if [ $command -eq 0 ] || [ $command -eq 30 ] || [ $command -eq 31 ] || [ $command -eq 32 ]; then
            read command
            echo "$command"
        fi
    else
        echo "$command"
    fi
done

