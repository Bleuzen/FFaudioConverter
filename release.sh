#!/bin/sh 
VERSION=$(awk '{if(/QCoreApplication::setApplicationVersion/) print $0}' src/main.cpp)
VERSION=${VERSION/QCoreApplication::setApplicationVersion(\"}
VERSION=${VERSION/\");}
VERSION=$(echo $VERSION | tr -d '\n')

tar -cJf FFaudioConverter-$VERSION.tar.xz \
    src/ \
    FFaudioConverter.pro \
    build.sh \
    ffaudioconverter.desktop
