#!/bin/sh
mkdir -p build
cd build/

qmake PREFIX=/usr ../FFaudioConverter.pro -spec macx-clang CONFIG+=release
make
