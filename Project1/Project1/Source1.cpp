#include "Header.h"
#include "Header1.h"
#include<array>

class Fuck : public Window {
	std::string ClassName() override { return "STATIC"; }
};

class Button : public Window {
public:
	std::string ClassName() override { return "Button"; }
};

class Edit : public Window {
public:
	std::string ClassName() override { return "Edit"; }
};

class ListBox : public Window {
public:
	std::string ClassName() override { return "ListBox"; }
};

class MainWindow : public Window
{
protected:
	//int OnCreate(CREATESTRUCT* pcs);
	//void OnCommand(int id);
	//void OnDestroy();
	RECT rect;
	Fuck st;
	POINT currpos;
	char command[10];
	char numbers[10];
	ListBox BL;
	Edit e;
	Button execute;
	Button remove;
	Button add;
	Button create;
	int x,i,j,y = 0;
	int numerical_value;

	int OnCreate(CREATESTRUCT* pcs)
	{
		add.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Add", IDC_ADD, 250, 130, 50, 30);
		remove.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Remove", IDC_REMOVE, 200, 100, 50, 30);
		execute.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Execute", IDC_EXECUTE, 200, 130, 50, 30);
		create.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Create Bot", IDC_CREATEBOT, 200, 160, 100, 30);
		e.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "", IDC_EDIT, 250, 100, 100, 30);
		BL.Create(*this, WS_CHILD | WS_VISIBLE, "", IDC_LB, 350, 100, 100, 100);
		EnableWindow(remove, false);
		EnableWindow(execute, false);
		GetClientRect(*this, &rect);
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
			GetWindowText(GetDlgItem(*this, IDC_EDIT), command, 10);
			if (command[0] == NULL)
				break;
			SendMessage(GetDlgItem(*this, IDC_LB), LB_ADDSTRING, NULL, (LPARAM)command);
			EnableWindow(GetDlgItem(*this, IDC_REMOVE), true);
			EnableWindow(GetDlgItem(*this, IDC_EXECUTE), true);
			SetDlgItemText(*this, IDC_EDIT, "");
			break;
		case IDC_EXECUTE:
			//get the first command in the list box, execute, and continue... move on to the next command
			//get the numeric values associated with the command
			//either apply the changes to the guibot window here, or send them elsewhere
			if (command[0] == NULL)
				break;
			y = SendMessage(GetDlgItem(*this, IDC_LB), LB_GETCOUNT, NULL, NULL);
			for (x;y > x;x++) 
			{
				SendMessage(GetDlgItem(*this, IDC_LB), LB_GETTEXT, x, (LPARAM)command);
				//number saver
				while (command[i] != NULL)
				{
					if (isdigit(command[i]))
					{
						numbers[j] = command[i];
						command[i] = NULL;
						j++;
					}
					i++;
				}
				//eats numbers up to 999... (- '0') to convert the char value to an integer ...ascii '0' = 48.. '1' = 49 ... 49 - 48 = 1
				if (j == 1)
					numerical_value = numbers[0] - '0';
				if (j == 2)
					numerical_value = (numbers[0] - '0') * 10 + (numbers[1] - '0');
				if (j == 3)
					numerical_value = (numbers[0] - '0') * 100 + (numbers[1] - '0') * 10 + (numbers[2] - '0');
				j = 0;
				i = 0;
				//moveguibot
				MoveGuiBot(numerical_value, command, currpos);
			}
			break;
		case IDC_CREATEBOT:
			if (!st)
				st.Create(*this, WS_CHILD | WS_VISIBLE | SS_CENTER, "GUIBOT", 0, 0, 0, 60, 15);
			SetWindowPos(st, 0, 600, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			currpos = {600,100};
			break;
		case IDC_REMOVE:
			int x = SendMessage(GetDlgItem(*this, IDC_LB), LB_GETCURSEL, NULL, NULL);
			if (x != LB_ERR)
				SendMessage(GetDlgItem(*this, IDC_LB), LB_DELETESTRING, x, NULL);
			x = SendMessage(GetDlgItem(*this, IDC_LB), LB_GETCOUNT, NULL, NULL);
			if (x == 0) {
				EnableWindow(GetDlgItem(*this, IDC_REMOVE), false);
				EnableWindow(GetDlgItem(*this, IDC_EXECUTE), false);
			}
			break;
		}
	}

	/*void OnLButtonDown(POINT p) {
		if (!st)
			st.Create(*this, WS_CHILD | WS_VISIBLE | SS_CENTER, "GUIBOT", 0, p.x, p.y, 60, 15);
		SetWindowPos(st, 0, p.x, p.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		currpos = p;
	}*/
	/*void OnKeyUp(int vk) {

		if (st) {
			SetWindowLong(st, GWL_STYLE, GetWindowLong(st, GWL_STYLE) & ~WS_BORDER);
			SetWindowPos(st, 0, currpos.x, currpos.y, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
		}
	}*/
	/*void OnKeyDown(int vk) {

		RECT rect;
		GetClientRect(*this, &rect);
		int movement;
		movement = (GetKeyState(VK_CONTROL) < 0) ? 10 : 1;
		switch (vk) {
		case VK_LEFT: currpos.x = max(currpos.x - movement, rect.left); break;
		case VK_RIGHT:currpos.x = min(currpos.x + movement, rect.right - 20); break;
		case VK_UP: currpos.y = max(currpos.y - movement, rect.top); break;
		case VK_DOWN: currpos.y = min(currpos.y + movement, rect.bottom - 20); break;
		}
		SetWindowLong(st, GWL_STYLE, GetWindowLong(st, GWL_STYLE) | WS_BORDER);
		SetWindowPos(st, 0, currpos.x, currpos.y, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER);
	}*/

	void MoveGuiBot(int numerical_value, char command[], POINT &currpos) {
		//FIRST TWO LETTERS OF EVERY COMMAND
		if (command[0] == 'u' && command[1] == 'p') //up
		{
			currpos.y = max(currpos.y - numerical_value, rect.top);
			SetWindowPos(st, 0, currpos.x, currpos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
		if (command[0] == 'd' && command[1] == 'o') //down
		{
			currpos.y = min(currpos.y + numerical_value, rect.bottom - 60);
			SetWindowPos(st, 0, currpos.x, currpos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
		if (command[0] == 'l' && command[1] == 'e') //left
		{
			currpos.x = max(currpos.x - numerical_value, rect.left);
			SetWindowPos(st, 0, currpos.x, currpos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
		if (command[0] == 'r' && command[1] == 'i') //right
		{
			currpos.x = min(currpos.x + numerical_value, rect.right - 60);
			SetWindowPos(st, 0, currpos.x, currpos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
	}
	void OnDestroy() 
	{
		::PostQuitMessage(0);
	}
};

/*int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	Application app;
	MainWindow wnd;
	wnd.Create(NULL, WS_OVERLAPPEDWINDOW | WS_VISIBLE, "NWP 4");
	return app.Run();
}*/


/*void MainWindow::OnDestroy() {
	::PostQuitMessage(0);
}*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDM_V2));
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
