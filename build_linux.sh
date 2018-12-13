#!/bin/sh
mkdir -p build
cd build/

qmake PREFIX=/usr ../FFaudioConverter.pro -spec linux-g++
make
