#!/bin/bash
gcc -Wall -I ./ DemoGame.c main.c ./VEngine/Source/*.c -o DemoGame -lm -lSDL2 -lSDL2_ttf
if [ $? -eq 0 ];
then
echo -e "\e[32mBuild Successful!"
fi