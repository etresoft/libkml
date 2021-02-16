#/bin/sh

CONFIGURATION=$1
PLATFORM=$2
ARCH=$3

if [ "$PLATFORM" == "macosx" ]
then
  DEPLOYMENT="MACOSX_DEPLOYMENT_TARGET=$MACOSX_DEPLOYMENT_TARGET"
elif [ "$PLATFORM" == "iphoneos" ]
then
  DEPLOYMENT="IPHONEOS_DEPLOYMENT_TARGET=$IPHONEOS_DEPLOYMENT_TARGET"
fi

xcodebuild -verbose -configuration $CONFIGURATION -target kml_$PLATFORM archive ARCHS=$ARCH $DEPLOYMENT

