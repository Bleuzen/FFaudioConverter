#!/bin/sh
mkdir -p build
cd build/

qmake ../FFaudioConverter.pro -spec linux-g++ CONFIG+=qtquickcompiler
make

mkdir -p dist
mv FFaudioConverter dist/ffaudioconverter
