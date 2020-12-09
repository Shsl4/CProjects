#!/bin/bash
gcc -Wall -o TempsReaction main.c GlobalVariables.c Definitions.c -D _GNU_SOURCE
if [ $? -eq 0 ];
then
echo -e "\e[32mBuild Successful!"
fi
