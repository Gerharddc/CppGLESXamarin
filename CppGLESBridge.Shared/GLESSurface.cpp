#include "GLESSurface.h"
#include "SimpleRenderer.h"

using namespace Platform;
using namespace MAINNAMESPACE;

GLESSurface::GLESSurface(Windows::UI::Xaml::Controls::SwapChainPanel^ panel)
{
	swapChainPanel = panel;
}


GLESSurface::~GLESSurface()
{
	StopRenderLoop();
	DestroyRenderSurface();
}

void GLESSurface::OnPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// The SwapChainPanel has been created and arranged in the page layout, so EGL can be initialized.
	CreateRenderSurface();
	StartRenderLoop();
}

void GLESSurface::OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args)
{
	if (args->Visible && mRenderSurface != EGL_NO_SURFACE)
	{
		StartRenderLoop();
	}
	else
	{
		StopRenderLoop();
	}
}

void GLESSurface::OnSwapChainPanelSizeChanged(Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	// Size change events occur outside of the render thread.  A lock is required when updating
	// the swapchainpanel size
	//critical_section::scoped_lock lock(mSwapChainPanelSizeCriticalSection);
	mSwapChainPanelSize = { e->NewSize.Width, e->NewSize.Height };
}

void GLESSurface::GetSwapChainPanelSize(GLsizei* width, GLsizei* height)
{
	//critical_section::scoped_lock lock(mSwapChainPanelSizeCriticalSection);
	// If a custom render surface size is specified, return its size instead of
	// the swapchain panel size.
	if (mUseCustomRenderSurfaceSize)
	{
		*width = static_cast<GLsizei>(mCustomRenderSurfaceSize.Width);
		*height = static_cast<GLsizei>(mCustomRenderSurfaceSize.Height);
	}
	else
	{
		*width = static_cast<GLsizei>(mSwapChainPanelSize.Width);
		*height = static_cast<GLsizei>(mSwapChainPanelSize.Height);
	}
}

void GLESSurface::CreateRenderSurface()
{
	if (/*mOpenGLES && */mRenderSurface == EGL_NO_SURFACE)
	{
		//
		// A Custom render surface size can be specified by uncommenting the following lines.
		// The render surface will be automatically scaled to fit the entire window.  Using a
		// smaller sized render surface can result in a performance gain.
		//
		//mCustomRenderSurfaceSize = Size(340, 400);
		//mUseCustomRenderSurfaceSize = true;

		//mRenderSurface = mOpenGLES.CreateSurface(swapChainPanel, mUseCustomRenderSurfaceSize ? &mCustomRenderSurfaceSize : nullptr);
		mRenderSurface = mOpenGLES.CreateSurface(swapChainPanel, nullptr);
	}
}

void GLESSurface::DestroyRenderSurface()
{
	//if (mOpenGLES)
	//{
	mOpenGLES.DestroySurface(mRenderSurface);
	//}
	mRenderSurface = EGL_NO_SURFACE;
}

void GLESSurface::RecoverFromLostDevice()
{
	// Stop the render loop, reset OpenGLES, recreate the render surface
	// and start the render loop again to recover from a lost device.

	StopRenderLoop();

	{
		//critical_section::scoped_lock lock(mRenderSurfaceCriticalSection);

		DestroyRenderSurface();
		mOpenGLES.Reset();
		CreateRenderSurface();
	}

	StartRenderLoop();
}

void GLESSurface::StartRenderLoop()
{
	// If the render loop is already running then do not start another thread.
	if (mRenderLoopWorker != nullptr && mRenderLoopWorker->Status == Windows::Foundation::AsyncStatus::Started)
	{
		return;
	}

	// Create a task for rendering that will be run on a background thread.
	auto workItemHandler = ref new Windows::System::Threading::WorkItemHandler([this](Windows::Foundation::IAsyncAction ^ action)
	{
		//critical_section::scoped_lock lock(mRenderSurfaceCriticalSection);

		mOpenGLES.MakeCurrent(mRenderSurface);
		SimpleRenderer renderer;
		renderer.Init();

		while (action->Status == Windows::Foundation::AsyncStatus::Started)
		{
			GLsizei panelWidth = 0;
			GLsizei panelHeight = 0;

			GetSwapChainPanelSize(&panelWidth, &panelHeight);
			renderer.UpdateWindowSize(panelWidth, panelHeight);
			renderer.Draw();

			// The call to eglSwapBuffers might not be successful (i.e. due to Device Lost)
			// If the call fails, then we must reinitialize EGL and the GL resources.
			if (mOpenGLES.SwapBuffers(mRenderSurface) != GL_TRUE)
			{
				// XAML objects like the SwapChainPanel must only be manipulated on the UI thread.
				swapChainPanel->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High, ref new Windows::UI::Core::DispatchedHandler([=]()
				{
					RecoverFromLostDevice();
				}, CallbackContext::Any));

				return;
			}
		}
	});

	// Run task on a dedicated high priority background thread.
	mRenderLoopWorker = Windows::System::Threading::ThreadPool::RunAsync(workItemHandler, Windows::System::Threading::WorkItemPriority::High, Windows::System::Threading::WorkItemOptions::TimeSliced);
}

void GLESSurface::StopRenderLoop()
{
	if (mRenderLoopWorker)
	{
		mRenderLoopWorker->Cancel();
		mRenderLoopWorker = nullptr;
	}
}