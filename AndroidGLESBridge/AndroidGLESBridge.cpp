#include "SimpleRenderer.h"

static SimpleRenderer sr;

extern "C" {
	void on_surface_created()
	{
		sr.Init();
	}

	void on_surface_changed(int width, int height)
	{
		sr.UpdateWindowSize(width, height);
	}

	void on_draw_frame()
	{
		sr.Draw();
	}
}