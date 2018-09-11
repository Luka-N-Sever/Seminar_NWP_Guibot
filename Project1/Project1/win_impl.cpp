#include "win_app_dlg_class.h"
#include "Control_IDs.h"
#include <sstream>
#include <vector>
#include "resource.h"
#include <windows.h>

# define M_PI 3.14159265358979323846  /* pi */

class Button : public Window {
public:
	std::string ClassName() override { return "Button"; }
};

class Guibot : public Window {
public:
	std::string ClassName() override { return "STATIC"; }
};

class Static : public Window {
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

int PicDialog::IDD() {
	return IDD_DIALOG1;
}

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
	
	Static text1;
	Static text2;
	Static text3;
	Static text4;
	Static text5;
	Static text6;
	Static text7;
	
	void CreateBot(HWND hw, Guibot& bot, POINT& point);
	void Add(HWND wh);
	void Remove(HWND hw);
	void MoveGuiBot(Guibot& st, int nv, std::string commando, POINT& point);
	void Execute();
	void Loop(HWND wh);

	std::vector<struct step> steps;

	int OnCreate(CREATESTRUCT* pcs, HWND hw)
	{ 
		GetClientRect(*this, &rect);
		std::vector<std::string> commands = { "UP", "RIGHT", "DOWN", "LEFT" };

		add.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Add", IDC_ADD,  130, 75, 100, 38);
		remove.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Remove", IDC_REMOVE, 73, 305, 100, 38);
		execute.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Execute", IDC_EXECUTE,  130, 230, 100, 38);
		create.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Create Bot", IDC_CREATEBOT, 20, 230, 100, 38);
		loop.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Loop", IDC_LOOP,  130, 380, 100, 38);
		e.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "", IDC_EDIT, 20,75, 100, 20);
		e2.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "", IDC_EDIT2, 20, 398, 100, 20);
		BL.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL, "", IDC_LB, 130, 130, 100, 100);
		BL2.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "", IDC_LB2, 20, 130, 100, 100);
		
		text1.Create(*this, WS_CHILD | WS_VISIBLE, "Numeric Value:", IDC_STATIC, 20, 50, 85, 15);
		text2.Create(*this, WS_CHILD | WS_VISIBLE, "Select a Command:", IDC_STATIC1, 20, 110, 100, 15);
		text3.Create(*this, WS_CHILD | WS_VISIBLE, "Add your Command:", IDC_STATIC2, 130, 50, 100, 15);
		text4.Create(*this, WS_CHILD | WS_VISIBLE, "Remove a Selected Command:", IDC_STATIC3, 53, 280, 150, 15);
		text5.Create(*this, WS_CHILD | WS_VISIBLE, "Loop Your Commands a Number of Times:", IDC_STATIC4, 20, 355, 210, 15);
		text6.Create(*this, WS_CHILD | WS_VISIBLE, "Number of Loops:", IDC_STATIC5, 20, 377, 95, 15);
		text7.Create(*this, WS_CHILD | WS_VISIBLE, "Proof of Concept (Robo_Mov.cpp):", IDC_STATIC6, 270, 220, 170, 15);
		
		SendDlgItemMessage(hw, IDC_STATIC6, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		SendDlgItemMessage(hw, IDC_STATIC5, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		SendDlgItemMessage(hw, IDC_STATIC4, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		SendDlgItemMessage(hw, IDC_STATIC3, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		SendDlgItemMessage(hw, IDC_STATIC2, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		SendDlgItemMessage(hw, IDC_STATIC1, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		SendDlgItemMessage(hw, IDC_STATIC, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		SendDlgItemMessage(hw, IDC_ADD, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		SendDlgItemMessage(hw, IDC_REMOVE, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		SendDlgItemMessage(hw, IDC_EXECUTE, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		SendDlgItemMessage(hw, IDC_CREATEBOT, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		SendDlgItemMessage(hw, IDC_LOOP, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		SendDlgItemMessage(hw, IDC_LB, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		SendDlgItemMessage(hw, IDC_EDIT, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		SendDlgItemMessage(hw, IDC_EDIT2, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		SendDlgItemMessage(hw, IDC_LB2, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		
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
		case ID_INFO_ABOUT:
			MessageBox(NULL,  "Check out the Schematic for an explaination of the lines", "About", MB_ICONINFORMATION);
			break;
		case ID_INFO_SCHEMATIC: 
			{PicDialog p; 
			p.DoModal(0, *this);}
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

	void OnPaint(HDC hdc, HWND hw)      //RightLeg Outwards
	{
	    int CurrentAngle = 270;
		int TargetAngle = 290;
		int AngleDifferntial = 1;

		MoveToEx(hdc, 350, 250, NULL);	//Defines a Joint at 300,150
		LineTo(hdc, 350, 350);			//Define a Foot at 300,250
		
		POINT JOINT = { 350,250 };
		POINT FOOT = { 350,350 };
		int LENGTH = 100;               //predefined in leg class along with the POINTS
		
		for (; CurrentAngle != TargetAngle; CurrentAngle += AngleDifferntial) { 
			MoveToEx(hdc, JOINT.x, JOINT.y, NULL);	 			
			FOOT.x = JOINT.x + (LENGTH * cos(CurrentAngle*M_PI / 180));
			FOOT.y = JOINT.y - (LENGTH * sin(CurrentAngle*M_PI / 180));
			LineTo(hdc, FOOT.x, FOOT.y);	 			
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
	SetWindowPos(st, 0, 575, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	currpos = { 575,100 };
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
	
	POINT p = currpos;
	int dy = 0, dx = 0;
	if (commando[0] == 'U' && commando[1] == 'P') //up
	{
		currpos.y = max(currpos.y - numerical_value, rect.top);
		dy = -1;
	}
	if (commando[0] == 'D' && commando[1] == 'O') //down
	{
		currpos.y = min(currpos.y + numerical_value, rect.bottom - 15);
		dy = 1;
	}
	if (commando[0] == 'L' && commando[1] == 'E') //left
	{
		currpos.x = max(currpos.x - numerical_value, rect.left);
		dx = -1;
	}
	if (commando[0] == 'R' && commando[1] == 'I') //right
	{
		currpos.x = min(currpos.x + numerical_value, rect.right - 60);
		dx = 1;
	}
	int interval = 1000 / (dx != 0 ? abs(currpos.x - p.x) : abs(currpos.y - p.y));
	for (; p.x != currpos.x || p.y != currpos.y; p.y += dy, p.x += dx) {
		SetWindowPos(st, NULL, p.x, p.y, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
		Sleep(interval);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MENU)); //resource file for menues...
	Application app;
	MainWindow wnd;
	wnd.Create(NULL, WS_OVERLAPPEDWINDOW | WS_VISIBLE, "GUIBOT 1.0", (int)hMenu);
	return app.Run();
}

