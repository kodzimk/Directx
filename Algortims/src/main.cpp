#include"Win.h"
#include"Window.h"
#include"Timer.h"

#define WIDTH 800
#define HEIGHT 600

Timer timer;

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	CoInitialize(NULL);

	Window window(WIDTH, HEIGHT, hInstance);

	while (true)
	{
		if (const auto ecode = window.ProcessMessages())
		{
			return *ecode;
		}


		window.gfx->ClearBuffer(1.0f, 1.0f, 1.0f);
		window.DoFrame();
		window.gfx->EndFrame();
	}

	return -1;
}