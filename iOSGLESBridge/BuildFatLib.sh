#This script compiles the library for all the needed configurations,
#creates the neccessary fat library files and copies them to the required location

mkdir build
cd build
mkdir Debug
mkdir Release
cd ../

echo "Building the debug files for the simulator..."
xcodebuild -project iOSGLESBridge.xcodeproj -sdk iphonesimulator -arch i386 -configuration Debug clean build
cp build/Debug-iphonesimulator/libiOSGLESBridge.a build/Debug/libiOSGLESBridge-i386.a
xcodebuild -project iOSGLESBridge.xcodeproj -sdk iphonesimulator -arch x86_64 -configuration Debug clean build
cp build/Debug-iphonesimulator/libiOSGLESBridge.a build/Debug/libiOSGLESBridge-x86_64.a

#TODO: lido thinks i386 and x86_64 are the same in release
echo "Building the release file for the simulator..."
xcodebuild -project iOSGLESBridge.xcodeproj -sdk iphonesimulator -arch i386 -configuration Release clean build
cp build/Debug-iphonesimulator/libiOSGLESBridge.a build/Release/libiOSGLESBridge-i386.a
#xcodebuild -project iOSGLESBridge.xcodeproj -sdk iphonesimulator -arch x86_64 -configuration Release clean build
#cp build/Debug-iphonesimulator/libiOSGLESBridge.a build/Release/libiOSGLESBridge-x86_64.a

echo "Building the debug files for the device..."
xcodebuild -project iOSGLESBridge.xcodeproj -sdk iphoneos -arch arm64 -configuration Debug clean build
cp build/Debug-iphoneos/libiOSGLESBridge.a build/Debug/libiOSGLESBridge-arm64.a
xcodebuild -project iOSGLESBridge.xcodeproj -sdk iphoneos -arch armv7 -configuration Debug clean build
cp build/Debug-iphoneos/libiOSGLESBridge.a build/Debug/libiOSGLESBridge-armv7.a
xcodebuild -project iOSGLESBridge.xcodeproj -sdk iphoneos -arch armv7s -configuration Debug clean build
cp build/Debug-iphoneos/libiOSGLESBridge.a build/Debug/libiOSGLESBridge-armv7s.a

#TODO: lido thinks that arm64, armv7, armv7s are the same in release
echo "Building the release files for the device..."
#xcodebuild -project iOSGLESBridge.xcodeproj -sdk iphoneos -arch arm64 -configuration Release clean build
#cp build/Debug-iphoneos/libiOSGLESBridge.a build/Release/libiOSGLESBridge-arm64.a
#xcodebuild -project iOSGLESBridge.xcodeproj -sdk iphoneos -arch armv7 -configuration Release clean build
#cp build/Debug-iphoneos/libiOSGLESBridge.a build/Release/libiOSGLESBridge-armv7.a
xcodebuild -project iOSGLESBridge.xcodeproj -sdk iphoneos -arch armv7s -configuration Release clean build
cp build/Debug-iphoneos/libiOSGLESBridge.a build/Release/libiOSGLESBridge-armv7s.a

cd build

echo "Creating the debug fat file"
cd Debug
lipo -create libiOSGLESBridge-i386.a libiOSGLESBridge-x86_64.a libiOSGLESBridge-arm64.a libiOSGLESBridge-armv7.a libiOSGLESBridge-armv7s.a -output libiOSGLESBridge-Debug.a
cd ../
cp Debug/libiOSGLESBridge-Debug.a ./
#rm -r Debug

echo "Creating the release fat file"
cd Release
lipo -create libiOSGLESBridge-i386.a libiOSGLESBridge-armv7s.a -output libiOSGLESBridge-Release.a
cd ../
cp Release/libiOSGLESBridge-Release.a ./
#rm -r Release

echo "Copying to the C# project folder"
cp libiOSGLESBridge-Debug.a ../../CppGLESXamarin.iOS/
cp libiOSGLESBridge-Release.a ../../CppGLESXamarin.iOS/

echo "Done!"