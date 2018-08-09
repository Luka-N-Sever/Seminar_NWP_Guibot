#include "Header.h"
#include "Header1.h"
#include <math.h>
//#include <conio.h>
//#include <dos.h>
//#include <graphics.h>
#include <array>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
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

class MainWindow : public Window
{
protected:
	RECT rect;
	Guibot bot;
	POINT currpos;
	char command[100];
	char numbers[3];
	std::string commands[10]; //will set the length of this array according to the amount of commands in the txt file...
	ListBox BL;
	ListBox BL2;
	Edit e;
	Edit e2;
	Button execute;
	Button remove;
	Button add;
	Button create;
	Button loop;
	//NOTE TO SELF - add a loop function to loop the commands in the list box a number of times, add a speed for GUIBOT speed...
	void CreateBot(HWND hw, Guibot& bot, POINT& point);
	void Add(HWND wh);
	void Remove(HWND hw);
	void MoveGuiBot(Guibot& st, int nv, char chars[], POINT& point);
	void InterpretCommand();
	void Execute();
	void Loop();
	int NumberConverter(int numerals, char numbers[]);
	int i, j, k, x, number_of_numerals_in_numbers, z, number_of_characters_in_command, currselection = 0;
	int numerical_value;
	/*Re-design: Read the commands from a txt file...populate a list box with them...read the numerical values from an edit control.
	On 'add' concatenate the choosen command and the numerical value, put it in a second list box, that will be executed*/
	int OnCreate(CREATESTRUCT* pcs)
	{
		std::ifstream file("Commands.txt");
		std::string str;
		while (std::getline(file, str, '\n'))
			commands[k++] = str;
		file.close();

		k = 0;
			
		add.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Add", IDC_ADD, 250, 130, 50, 30);
		remove.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Remove", IDC_REMOVE, 200, 100, 50, 30);
		execute.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Execute", IDC_EXECUTE, 200, 130, 50, 30);
		create.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Create Bot", IDC_CREATEBOT, 200, 160, 100, 30);
		loop.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Loop", IDC_LOOP, 300, 130, 50, 30);
		e.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "", IDC_EDIT, 250, 100, 100, 30);
		e2.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "", IDC_EDIT2, 300, 160, 50, 30);
		BL.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "", IDC_LB, 350, 100, 100, 100);
		BL2.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "", IDC_LB2, 450, 100, 100, 100);
		EnableWindow(remove, false);
		EnableWindow(execute, false);
		//GetClientRect(*this, &rect);
		
		while (true)
		{
			if (commands[k]=="")
				break;
			SendMessage(GetDlgItem(*this, IDC_LB2), LB_ADDSTRING, NULL, (LPARAM)commands[k].c_str());
			k++;
		}
		k = 0;
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
			//should select the command that is currently being executed and highlight it for a better user experience
			Execute();												
			break;
		case IDC_CREATEBOT:
			CreateBot(*this, bot, currpos);
			break;
		case IDC_REMOVE:
			Remove(*this);
			break;
		case IDC_LOOP:
			Loop();
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
		for (angle = 270; angle <= 360; angle++){ //can put these points into a list of some sort
			MoveToEx(hdc, 100, 400, NULL);
			X = 100 + (100 * cos(angle*M_PI / 180)); 
			Y = 400 - (100 * sin(angle*M_PI / 180));
			LineTo(hdc, X, Y);
		}
	}
};

void MainWindow::Loop() {
	char loops[3];
	GetWindowText(GetDlgItem(*this, IDC_EDIT2), loops, 3);
	int number_of_numerals = 0;
	int loop_value;

	while (loops[number_of_numerals] != NULL) 
		number_of_numerals++;
	loop_value = NumberConverter(number_of_numerals, loops);

	for (int s = 0; s < loop_value; s++)
		Execute();
}

int MainWindow::NumberConverter(int numerals, char numbers[]) {
	int value;
	if (numerals == 1)
		value = numbers[0] - '0';
	if (numerals == 2)
		value = (numbers[0] - '0') * 10 + (numbers[1] - '0');
	if (numerals == 3)
		value = (numbers[0] - '0') * 100 + (numbers[1] - '0') * 10 + (numbers[2] - '0');
	return value;
}

void MainWindow::Execute() {
	z = SendMessage(GetDlgItem(*this, IDC_LB), LB_GETCOUNT, NULL, NULL);
	if (command[0] == NULL)
		return;
	for (x = 0;z > x;x++)
	{
		InterpretCommand();
		MoveGuiBot(bot, numerical_value, command, currpos);//BREAK
	}
}

void MainWindow::InterpretCommand() {

	SendMessage(GetDlgItem(*this, IDC_LB), LB_GETTEXT, x, (LPARAM)command);
	
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
	numerical_value = NumberConverter(j, numbers);
	j = 0;
	i = 0;
}

void MainWindow::Add(HWND wh) {
	number_of_characters_in_command = 0; number_of_numerals_in_numbers = 0;
	currselection = SendMessage(GetDlgItem(*this, IDC_LB2), LB_GETCURSEL, NULL, NULL);
	SendMessage(GetDlgItem(*this, IDC_LB2), LB_GETTEXT, currselection, (LPARAM)command);
	GetWindowText(GetDlgItem(*this, IDC_EDIT), numbers, 10);
	while (true)
	{
		if (command[number_of_characters_in_command] == '\0')
			break;
		number_of_characters_in_command++; //starting from 1 QED "GUIBOT" = 6
	}
	while (true)
	{
		if (numbers[number_of_numerals_in_numbers] == '\0')
			break;
		number_of_numerals_in_numbers++;
	}
	command[number_of_characters_in_command] = ' '; //HA So there's a nice looking white space 
	number_of_characters_in_command += 1;
	for (int i = 0; i <= number_of_numerals_in_numbers; i++, x++)
	{
		command[number_of_characters_in_command++] = numbers[i];
	}
	SendMessage(GetDlgItem(*this, IDC_LB), LB_ADDSTRING, NULL, (LPARAM)command);
	//should add an if here so these fuckers dont turn on without there being any commands
	if (!(isalpha(command[0])))
		return;
	EnableWindow(GetDlgItem(*this, IDC_REMOVE), true);
	EnableWindow(GetDlgItem(*this, IDC_EXECUTE), true);
}

void MainWindow::CreateBot(HWND hw, Guibot& st, POINT& currpos) {
	if (!st)
		st.Create(hw, WS_CHILD | WS_VISIBLE | SS_CENTER, "GUIBOT", 0, 0, 0, 60, 15);
	SetWindowPos(st, 0, 600, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	currpos = { 600,100 };
}

void MainWindow::Remove(HWND wh) {
	int z = 0;
	z = SendMessage(GetDlgItem(wh, IDC_LB), LB_GETCURSEL, NULL, NULL);
	if (z != LB_ERR)
		SendMessage(GetDlgItem(wh, IDC_LB), LB_DELETESTRING, z, NULL);
	z = SendMessage(GetDlgItem(wh, IDC_LB), LB_GETCOUNT, NULL, NULL);
	if (z == 0) {
		EnableWindow(GetDlgItem(wh, IDC_REMOVE), false);
		EnableWindow(GetDlgItem(wh, IDC_EXECUTE), false);
	}
}

void MainWindow::MoveGuiBot(Guibot& st, int numerical_value, char command[], POINT& currpos) {
	
	if (command[0] == 'U' && command[1] == 'P') //up
	{
		for (float f = 0; f <= numerical_value; f += 0.001) {
			SetWindowPos(st, NULL, currpos.x, currpos.y - f, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
		}
		currpos.y = max(currpos.y - numerical_value, rect.top);
	}
	if (command[0] == 'D' && command[1] == 'O') //down
	{
		for (float f = 0; f <= numerical_value; f += 0.001) {
			SetWindowPos(st, NULL, currpos.x, currpos.y + f, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
		}
		currpos.y = min(currpos.y + numerical_value, rect.bottom - 15);
	}
	if (command[0] == 'L' && command[1] == 'E') //left
	{
		for (float f = 0; f <= numerical_value; f += 0.001) {
			SetWindowPos(st, 0, currpos.x - f, currpos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
		currpos.x = max(currpos.x - numerical_value, rect.left);
	}
	if (command[0] == 'R' && command[1] == 'I') //right
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
/*U redu, pokupio sam trenutno stanje repozitorija i projekt se build-a.

Ali, kao što smo se dogovorili, prvo dovedite projekt u stanje da se ista
moze pratiti - fileovi se zovu source/header, sve funkcije su u tijelu
klase, funkcije su prevelike i ima brdo zakomentiranog koda. Javite se kad
budete unijeli promjene i tada napisite sto bi (i kako) trebalo raditi.

Za pocetak: 
- nadam se da necete imati (neinicijalizirane) membere koji se zovu i, j,
z (?!)
- ima puno previse konstanti po kodu
- mozete koristiti std::string umjesto char[10] (a zasto ne koristite
Unicode?)

Pozdrav
Nenad Caklovic*/