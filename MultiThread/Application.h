#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <windows.h>
//#include "GraphicsEngine.h"
class SampleApp
{
public:
	SampleApp(HINSTANCE hInstance);
	~SampleApp();
	void Run();
	LRESULT ProcessWindowMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	void InitWindow(int width, int length, DWORD style);

	HINSTANCE m_hInstance;
	HWND m_hWnd;
};

extern SampleApp* gApp;

#endif