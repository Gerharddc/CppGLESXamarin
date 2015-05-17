This is a template Visual Studio solution to use C++ OpenGL inside C# (Xamarin) application. It was made using Visual Studio 2015 Community RC.

At the moment it supports Windows 8.1 (Store), Windows Phone 8.1 and Android.

The OpenGL ES code is in a shared file which is then absorbed into native C# application.
For the Windows platforms a C++ runtime component is used to bridge into C# whilst on Android a shared library and P/Invoke is used.

Both iOS and Xamarin.Forms support is planned but this can only happen after someone with a Mac contributes and after Xamarin.Forms starts working on Android with VS 2015.
