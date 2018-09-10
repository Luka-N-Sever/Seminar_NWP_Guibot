#include <windows.h>
#include <tchar.h>
#include "Dlg_def.h"
#include "resource_IDs.h"
#include <map>

int CALLBACK Dialog::Proc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{ 
	
	HBITMAP bitmap = NULL;
	HDC hdcMem;
	PAINTSTRUCT ps;
	HDC hdc = GetDC(hw);
	static std::map<HWND, Dialog*> wmap;
	
	if (msg == WM_PAINT) 
	{
		bitmap = (HBITMAP)LoadImage(GetModuleHandle("Project1.exe"), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, 0);
		hdc = BeginPaint(hw, &ps);
		hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, bitmap);
		StretchBlt(
			hdc,
			0,
			0,
			533,
			288,
			hdcMem,
			0,
			0,
			3456,
			2464,
			SRCCOPY);
		EndPaint(hw, &ps);
	}
	if (msg == WM_INITDIALOG)
	{
		Dialog* pThis = reinterpret_cast<Dialog*>(lp);
		pThis->hw = hw;
		wmap[hw] = pThis;
		return pThis->OnInitDialog(hw);
	}
	Dialog* pThis = wmap[hw];
	if (msg == WM_COMMAND)
	{
		if (LOWORD(wp) == IDOK && pThis->OnOK())
			return EndDialog(hw, IDOK);
		if (LOWORD(wp) == IDCANCEL)
		{
			pThis->OnCancel();
			return EndDialog(hw, IDCANCEL);
		}
		return pThis->OnCommand(LOWORD(wp), HIWORD(wp));
	}
	if (msg == WM_DESTROY)
	{
		DeleteObject(bitmap);
		wmap.erase(hw);
	}
//WM_GETICON .... 127
	return 0;
}

int Dialog::DoModal(HINSTANCE hInst, HWND parent)
{
	return DialogBoxParam(hInst, MAKEINTRESOURCE(IDD()), parent, Proc,
		reinterpret_cast<LPARAM>(this));
}