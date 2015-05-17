using Android.Content;
using Android.Views;
using Android.Opengl;
using Android.Util;

namespace CppGLESXamarin.Android
{
    public class MyGLSurfaceView : GLSurfaceView
    {
        private MyGLRenderer mRenderer;

        public MyGLSurfaceView(Context context, IAttributeSet attrs) : base(context, attrs)
        {
            //Emulator fix
            SetEGLConfigChooser(8, 8, 8, 8, 16, 0);

            // Create an OpenGL ES 2.0 context.
            SetEGLContextClientVersion(2);

            // Set the Renderer for drawing on the GLSurfaceView
            mRenderer = new MyGLRenderer();
            SetRenderer(mRenderer);
        }
    }
}