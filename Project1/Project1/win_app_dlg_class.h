#pragma once
#include <windows.h>
#include <string>
#include "Dlg_def.h"


class Application
{
public:
	int Run();
};

class PicDialog : public Dialog {

protected:
	int IDD();
	bool OnInitDialog(HWND hw);
};

class Window
{
	HWND hw;

protected:
	virtual std::string ClassName();
	bool Register(const std::string& name);
	std::string GenerateClassName();

public:
	Window();

	bool Create(HWND parent, DWORD style, PCSTR caption = 0, int IdOrMenu = 0,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);

	operator HWND();
	static LRESULT CALLBACK Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	//	messages
protected:
	virtual int OnCreate(CREATESTRUCT*, HWND hw) { return 0; }
	virtual void OnCommand(int) {}
	virtual void OnDestroy() {}
	virtual void OnTimer(WPARAM) {}
	virtual void OnPaint(HDC hdc, HWND hw) {}
	/*virtual void OnLButtonDown(POINT p) { }*/
	/*virtual void OnKeyDown(int vk) { }
	virtual void OnKeyUp(int vk) { }*/
};
