#include "windows.h"
#include "Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	SampleApp app(hInstance);
	gApp = &app;
	app.Run();
}