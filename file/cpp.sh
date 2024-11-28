#!/bin/bash

if [[ -z "$1" ]]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

cp /home/fathi/Learning/scripts/file/main.cpp "$1.cpp"