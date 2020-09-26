#!/bin/bash

while read command
do
    grep -oh "\w*\s*[^(IFJ)]$command" scanner.h 
done

