#pragma once
#include <string>
class XCtrl {};

typedef std::basic_string<TCHAR> tstring;

class Dialog
{
	HWND hw;
public:
	static int CALLBACK Proc(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	operator HWND() const { return hw; }
	int DoModal(HINSTANCE, HWND parent = 0);
protected:
	//	overridables
	virtual int IDD() = 0;
	virtual bool OnInitDialog(HWND hw) { return false; }
	virtual bool OnCommand(int id, int code) { return false; }
	virtual bool OnOK() { return true; }
	virtual void OnCancel() { }
};
#pragma once
