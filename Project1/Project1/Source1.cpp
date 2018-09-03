#include "Header.h"
#include "Header1.h"
#include <math.h>
//#include <conio.h>
//#include <dos.h>
//#include <graphics.h>
#include <sstream>
#include <string>
#include <array>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
//#include "Commands.txt"
# define M_PI 3.14159265358979323846  /* pi */

class Button : public Window {
public:
	std::string ClassName() override { return "Button"; }
};

class Guibot : public Window {
public:
	std::string ClassName() override { return "STATIC"; }
};

class Edit : public Window {
public:
	std::string ClassName() override { return "Edit"; }
};

class ListBox : public Window {
public:
	std::string ClassName() override { return "ListBox"; }
};

struct step { 
	std::string cmnd;
	int val;
	void setcmnd(std::string command);
	void setvalue(int value);

	step() {};
	step(std::string cmnd, int value): cmnd(cmnd), val(val) {}
	std::string static tostring(step step) { std::stringstream ss; ss << step.val; std::string str = step.cmnd + ' ' + ss.str(); return str; }
};
	
void step::setcmnd(std::string command) { cmnd = command; };
void step::setvalue(int value) { val = value; };

class MainWindow : public Window
{
protected:
	RECT rect;
	Guibot bot;
	POINT currpos;
	ListBox BL;
	ListBox BL2;
	Edit e;
	Edit e2;
	Button execute;
	Button remove;
	Button add;
	Button create;
	Button loop;
	//NOTE TO SELF -  add a speed cont for GUIBOT speed...
	void CreateBot(HWND hw, Guibot& bot, POINT& point);
	void Add(HWND wh);
	void Remove(HWND hw);
	void MoveGuiBot(Guibot& st, int nv, std::string commando, POINT& point);
	void Execute();
	void Loop(HWND wh);
	
	//ADD A CLEAR CONTROL
	std::vector<struct step> steps;
	
	int OnCreate(CREATESTRUCT* pcs, HWND hw)
	{ 
		
		std::vector<std::string> commands = { "UP", "RIGHT", "DOWN", "LEFT" };

		add.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Add", IDC_ADD, 250, 130, 50, 30);
		remove.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Remove", IDC_REMOVE, 200, 100, 50, 30);
		execute.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Execute", IDC_EXECUTE, 200, 130, 50, 30);
		create.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Create Bot", IDC_CREATEBOT, 200, 160, 100, 38);
		loop.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Loop", IDC_LOOP, 300, 130, 50, 30);
		e.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "", IDC_EDIT, 250, 100, 100, 30);
		e2.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "", IDC_EDIT2, 300, 160, 50, 38);
		BL.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "", IDC_LB, 350, 100, 100, 100);
		BL2.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "", IDC_LB2, 450, 100, 100, 100);
		EnableWindow(remove, false);
		EnableWindow(execute, false);
		GetClientRect(*this, &rect);
	
		for (std::vector<std::string>::iterator it = commands.begin(); it != commands.end(); ++it)
			SendDlgItemMessage(hw, IDC_LB2, LB_ADDSTRING, NULL, (LPARAM) it->c_str());

		return 0;
	}

	void OnCommand(int id) {
		switch (id)
		{
		case ID_FILE_EXIT:
			OnDestroy();
			break;
		case ID_HELP_ABOUT:
			MessageBox(*this, "You can do some things here, I hope...", "About", MB_ICONINFORMATION);
			break;
		case IDC_ADD:
			Add(*this);
			break;
		case IDC_EXECUTE:
			Execute();												
			break;
		case IDC_CREATEBOT:
			CreateBot(*this, bot, currpos);
			break;
		case IDC_REMOVE:
			Remove(*this);
			break;
		case IDC_LOOP:
			Loop(*this);
			break;
		}
	}

	void OnDestroy()
	{
		::PostQuitMessage(0);
	}
	void OnPaint(HDC hdc, HWND hw) {
		float X, Y;
		int angle;
		MoveToEx(hdc, 100, 400, NULL);
		LineTo(hdc, 200, 400);
		for (angle = 270; angle <= 360; angle++) { //can put these points into a list of some sort
			MoveToEx(hdc, 100, 400, NULL);
			X = 100 + (100 * cos(angle*M_PI / 180));
			Y = 400 - (100 * sin(angle*M_PI / 180));
			LineTo(hdc, X, Y);
		}
	}
};

void MainWindow::Loop(HWND wh) {

	int loop_value = GetDlgItemInt(wh, IDC_EDIT2, NULL, FALSE);
	
	for (int s = 0; s < loop_value; s++)
		Execute();
}

void MainWindow::Execute() {

	if (!(steps.size()))
		return;
	for (std::vector<step>::iterator it = steps.begin(); it != steps.end(); ++it)
		MoveGuiBot(bot, it->val, it->cmnd, currpos);
}

void MainWindow::Add(HWND wh) {
	step step;
	std::string str2;
	std::stringstream ss;
	int currselection = SendDlgItemMessage(wh, IDC_LB2, LB_GETCURSEL, NULL, NULL);

	SendDlgItemMessage(wh, IDC_LB2, LB_GETTEXT, currselection, (LPARAM) str2.c_str());
	ss << str2.c_str();
	ss >> step.cmnd;
	step.setvalue(GetDlgItemInt(wh, IDC_EDIT, NULL, FALSE));
	steps.push_back(step); 

	std::string str = step::tostring(step);
	SendDlgItemMessage(wh, IDC_LB, LB_ADDSTRING, NULL, (LPARAM)str.c_str());
	if (steps.size()) {
		EnableWindow(GetDlgItem(*this, IDC_REMOVE), true);
		EnableWindow(GetDlgItem(*this, IDC_EXECUTE), true);
	}
}

void MainWindow::CreateBot(HWND hw, Guibot& st, POINT& currpos) {
	if (!st)
		st.Create(hw, WS_CHILD | WS_VISIBLE | SS_CENTER, "GUIBOT", 0, 0, 0, 60, 15);
	SetWindowPos(st, 0, 600, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	currpos = { 600,100 };
}

void MainWindow::Remove(HWND wh) {
	int LB_CurSel = SendDlgItemMessage(wh, IDC_LB, LB_GETCURSEL, NULL, NULL);
	if (LB_CurSel != LB_ERR) {
		SendDlgItemMessage(wh, IDC_LB, LB_DELETESTRING, LB_CurSel, NULL);
		steps.erase(steps.begin() + LB_CurSel);
	}	
	LB_CurSel = SendDlgItemMessage(wh, IDC_LB, LB_GETCOUNT, NULL, NULL);
	if (LB_CurSel == 0) {
		EnableWindow(GetDlgItem(wh, IDC_REMOVE), false);
		EnableWindow(GetDlgItem(wh, IDC_EXECUTE), false);
	}
}

void MainWindow::MoveGuiBot(Guibot& st, int numerical_value, std::string commando, POINT& currpos) {
	
	if (commando[0] == 'U' && commando[1] == 'P') //up
	{
		for (float f = 0; f <= numerical_value; f += 0.001) {
			SetWindowPos(st, NULL, currpos.x, currpos.y - f, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
		}
		currpos.y = max(currpos.y - numerical_value, rect.top);
	}
	if (commando[0] == 'D' && commando[1] == 'O') //down
	{
		for (float f = 0; f <= numerical_value; f += 0.001) {
			SetWindowPos(st, NULL, currpos.x, currpos.y + f, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
		}
		currpos.y = min(currpos.y + numerical_value, rect.bottom - 15);
	}
	if (commando[0] == 'L' && commando[1] == 'E') //left
	{
		for (float f = 0; f <= numerical_value; f += 0.001) {
			SetWindowPos(st, 0, currpos.x - f, currpos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
		currpos.x = max(currpos.x - numerical_value, rect.left);
	}
	if (commando[0] == 'R' && commando[1] == 'I') //right
	{
		for (float f = 0; f <= numerical_value; f += 0.001) {
			SetWindowPos(st, 0, currpos.x + f, currpos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
		currpos.x = min(currpos.x + numerical_value, rect.right - 60);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDM_V2)); //resource file for menues...
	MainWindow wnd;
	wnd.Create(NULL, WS_OVERLAPPEDWINDOW | WS_VISIBLE, "GUIBOT 0.0", (int)hMenu);
	// set icons
	HICON hib = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_V2), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	PostMessage(wnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hib));
	HICON his = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_V2), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	PostMessage(wnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(his));
	Application app;
	return app.Run();
}
