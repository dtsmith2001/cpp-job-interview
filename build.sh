#!/usr/bin/env bash

rm -f count count.o
g++ -Wno-c++11-extensions -c -I/usr/local/include count.cpp && g++ count.o -o count
