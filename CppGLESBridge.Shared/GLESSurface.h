#pragma once

#include "OpenGLES.h"

#include <concrt.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

//We need a different namespace for the Phone and Store versions
#ifdef WIN_STORE
#define MAINNAMESPACE CppGLESBridge_Win
#else
#define MAINNAMESPACE CppGLESBridge_WP
#endif

namespace MAINNAMESPACE
{
	public ref class GLESSurface sealed
	{
	public:
		GLESSurface(Windows::UI::Xaml::Controls::SwapChainPanel^ panel);
		virtual ~GLESSurface();

		void OnPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
		void OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void GetSwapChainPanelSize(GLsizei* width, GLsizei* height);

	private:
		void CreateRenderSurface();
		void DestroyRenderSurface();
		void RecoverFromLostDevice();
		void StartRenderLoop();
		void StopRenderLoop();

		OpenGLES mOpenGLES;
		Windows::UI::Xaml::Controls::SwapChainPanel^ swapChainPanel;

		Windows::Foundation::Size mSwapChainPanelSize;
		Concurrency::critical_section mSwapChainPanelSizeCriticalSection;

		Windows::Foundation::Size mCustomRenderSurfaceSize;
		bool mUseCustomRenderSurfaceSize;

		EGLSurface mRenderSurface;     // This surface is associated with a swapChainPanel on the page
		Concurrency::critical_section mRenderSurfaceCriticalSection;
		Windows::Foundation::IAsyncAction^ mRenderLoopWorker;
	};
}

