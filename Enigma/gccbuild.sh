#!/bin/bash
gcc -Wall -o Enigma main.c GlobalVariables.c Definitions.c libSl4sh.c
if [ $? -eq 0 ];
then
echo -e "\e[32mBuild Successful!"
fi
