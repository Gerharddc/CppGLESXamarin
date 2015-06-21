using System;
using System.Runtime.InteropServices;
using CoreGraphics;
using UIKit;
using GLKit;
using OpenGLES;

namespace CppGLESXamarin.iOS
{
	class GLESBridge
    {
        [DllImport("__Internal")]
        public static extern void on_surface_created();

        [DllImport("__Internal")]
        public static extern void on_surface_changed(int width, int height);

        [DllImport("__Internal")]
        public static extern void on_draw_frame();
    }

	class GLESViewController : GLKViewController, IGLKViewDelegate
	{
		EAGLContext context { get; set; }

		public override void ViewDidLoad()
		{
			base.ViewDidLoad();

			context = new EAGLContext(EAGLRenderingAPI.OpenGLES2);

			if (context == null)
			{
				throw new Exception("Failed to create an EAGL context");
			}

			var view = (GLKView) View;
			view.Context = context;
			view.DrawableDepthFormat = GLKViewDrawableDepthFormat.Format24;

			SetupGL();
		}

		protected override void Dispose(bool disposing)
		{
			base.Dispose(disposing);

			TearDownGL();

			if (EAGLContext.CurrentContext == context)
			{
				EAGLContext.SetCurrentContext(null);
			}
		}

		public override void DidReceiveMemoryWarning()
		{
			base.DidReceiveMemoryWarning();

			if (IsViewLoaded && View.Window == null)
			{
				View = null;

				TearDownGL();

				if (EAGLContext.CurrentContext == context)
				{
					EAGLContext.SetCurrentContext(null);
				}
			}
		}

		void SetupGL()
		{
			EAGLContext.SetCurrentContext(context);

			GLESBridge.on_surface_created();
		}

		void TearDownGL()
		{
			EAGLContext.SetCurrentContext(context);
			//TODO: this should be implemented in the C++
		}

		#region GLKView and GLKViewController delegate methods

		public override void Update()
		{
			//base.Update(); //This seems to cause some problems

			//TODO: this should actually only be called when the size has changed
			GLESBridge.on_surface_changed((int)View.Bounds.Size.Width, (int)View.Bounds.Size.Height);
		}

		public void DrawInRect(GLKView view, CGRect rect)
		{
			GLESBridge.on_draw_frame();
		}

		#endregion
	}

	public partial class ViewController : UIViewController
	{
		public ViewController (IntPtr handle) : base (handle)
		{
		}

		public override void ViewDidLoad ()
		{
			base.ViewDidLoad ();
			// Perform any additional setup after loading the view, typically from a nib.

			//TODO: you should change this part to suit your needs
			GLESViewController gvc = new GLESViewController();
			gvc.View.Layer.Frame = new CGRect(30, 30, View.Frame.Width - 60, View.Frame.Height - 60);
			gvc.View.Layer.BorderColor = UIColor.Blue.CGColor;
			gvc.View.Layer.BorderWidth = 2.0f;
			AddChildViewController(gvc);
			View.AddSubview(gvc.View);
		}

		public override void DidReceiveMemoryWarning ()
		{
			base.DidReceiveMemoryWarning ();
			// Release any cached data, images, etc that aren't in use.
		}
	}
}