#!/bin/bash
gcc -Wall -o GameOfLife main.c GlobalVariables.c Definitions.c
if [ $? -eq 0 ];
then
echo -e "\e[32mBuild Successful!"
fi
