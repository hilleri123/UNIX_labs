#!/bin/bash

g++ main.cpp -ldl -o Run

#gcc -Wall -fPIC -c lib.c
#gcc -shared -o lib.so lib.o
g++ -Wall -fPIC -c lib.cpp
g++ -shared -o lib.so lib.o
rm lib.o
