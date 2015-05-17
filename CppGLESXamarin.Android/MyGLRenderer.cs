using System.Runtime.InteropServices;
using Android.Opengl;

namespace CppGLESXamarin.Android
{
    class MyGLRenderer : Java.Lang.Object, GLSurfaceView.IRenderer
    {
        [DllImport("libAndroidGLESBridge.so")]
        public static extern void on_surface_created();

        [DllImport("libAndroidGLESBridge.so")]
        public static extern void on_surface_changed(int width, int height);

        [DllImport("libAndroidGLESBridge.so")]
        public static extern void on_draw_frame();

        #region IRenderer implementation
        public void OnDrawFrame(Javax.Microedition.Khronos.Opengles.IGL10 gl)
        {
            on_draw_frame();
        }

        public void OnSurfaceChanged(Javax.Microedition.Khronos.Opengles.IGL10 gl, int width, int height)
        {
            on_surface_changed(width, height);
        }

        public void OnSurfaceCreated(Javax.Microedition.Khronos.Opengles.IGL10 gl, Javax.Microedition.Khronos.Egl.EGLConfig config)
        {
            on_surface_created();
        }
        #endregion
    }
}