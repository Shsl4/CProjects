#!/bin/bash
rm imgSort
gcc -Wall -o imgSort main.c Source/*.c -lm -g
if [ $? -eq 0 ];
then
echo -e "\e[32mBuild Successful!"
fi
