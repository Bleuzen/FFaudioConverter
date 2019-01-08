#!/bin/sh
mkdir -p build
cd build/

qmake PREFIX=/usr ../FFaudioConverter.pro -spec linux-g++ CONFIG+=release
make
strip -s ffaudioconverter
