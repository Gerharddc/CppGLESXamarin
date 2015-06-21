This is a template Visual Studio solution to use C++ OpenGL inside C# (Xamarin) application. It was made using Visual Studio 2015 Community RC.

At the moment it supports Windows 8.1 (Store), Windows Phone 8.1 and Android.

The OpenGL ES code is in a shared file which is then absorbed into native C# application.
For the Windows platforms a C++ runtime component is used to bridge into C# whilst on Android a shared library and P/Invoke is used.

Both iOS and Xamarin.Forms support is planned but this can only happen after someone with a Mac contributes and after Xamarin.Forms starts working on Android with VS 2015.

THIS BRANCH:
iOS support is almost finished in this branch.
Working with iOS is slightly more complicated than on the other platforms due to the complication in building the C++ static libraries. An XCode project utilizing the shared C++ source files has been made for this. It also includes the iOS specific C++ wrapper. 
The largest complication is that of building the actual library file. This is easier done from the terminal than from XCode itself. The problem is that we need to support many different architectures and therefore require a combined "fat" library. There is a scipt called "BuildFatLib.sh" for this in the iOSGLESBridge folder. Simply run this script to compile the fat libraries. It compiles all the files, combines them and then copies them to the C# iOS project directory from where it will be linked. It is currently quite hardcoded and will require a bit of rework for any new project.
At the moment release builds are not supported yet because the script fails to create libraries for the correct architecture in release mode. I am not sure why yet but it seem that in release mode, xcodebuild ignores the arch flag and defaults to whatever it likes. I am trying to fix this and would appreciate any help with that.
You currently have to run the library build process on a Mac and then copy the resulting files over to Windows if you want to use Visual Studio. Maybe someone who has the time can create a nice remote control application for this that can be run from Visual Studio during the build process... In due time though, the native iOS C++ on VS 2015 should reach completion whereafter it will hopefully be able to do this by itself.