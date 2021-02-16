#/bin/sh

PREFIX=$1
CONFIGURATION=$2
PLATFORM=$3
ARCH=$4

mkdir -p $PREFIX/lib

# Copy libraries into $PREFIX
cp install/lib/libkml.a $PREFIX/lib

# Copy all headers
mkdir -p $PREFIX/include/kml/dom
mkdir -p $PREFIX/include/kml/xsd
mkdir -p $PREFIX/include/kml/convenience
mkdir -p $PREFIX/include/kml/engine
mkdir -p $PREFIX/include/kml/base
mkdir -p $PREFIX/include/kml/regionator

cp src/kml/*.h $PREFIX/include/kml
cp src/kml/dom/*.h $PREFIX/include/kml/dom
cp src/kml/xsd/*.h $PREFIX/include/kml/xsd
cp src/kml/convenience/*.h $PREFIX/include/kml/convenience
cp src/kml/engine/*.h $PREFIX/include/kml/engine
cp src/kml/base/*.h $PREFIX/include/kml/base
cp src/kml/regionator/*.h $PREFIX/include/kml/regionator

cp libkml.$PLATFORM.$ARCH.pc $PREFIX/lib/pkgconfig/libkml.pc