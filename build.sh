#!/bin/sh
mkdir -p build-release
cd build/

qmake ../FFaudioConverter.pro -spec linux-g++ CONFIG+=qtquickcompiler
make

mkdir -p dist
mv FFaudioConverter dist/
