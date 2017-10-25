// Will Hinds, Computer Games Engineering MSc - UI.h //
// ------------------------------------------------- //
// Contains UI code                                  //

#pragma once

#include <iostream>
#include <string>

using namespace std;

class Terminal
{
public:
	Terminal();
	~Terminal() { }
	inline void ClearScreen() { system("cls"); }
	inline void Forward() { cout << ' '; ++pos[0]; }
	inline void Down() { cout << endl; ++pos[1]; pos[0] = 0; }
	void HLine(char edge, char c);
	void Center(char edge, string text);
	void TitleBox(string title);
	void AddText(string text);
	bool GetResponse();
	int GetNum();
	float GetFloat();
	string GetString();
private:
	int width = 80;
	int height = 80;
	int pos[2];
};

Terminal::Terminal()
{
	ClearScreen();
	pos[0] = 0;
	pos[1] = 0;
}

void Terminal::TitleBox(string title)
{
	HLine('+', '=');
	HLine('|', ' ');
	Center('|', title);
	HLine('|', ' ');
	HLine('+', '=');
}

void Terminal::AddText(string text)
{
	if (text.size() > 78)
		return;
	HLine(' ', ' ');
	Center(' ', text);
	HLine(' ', ' ');
}

bool Terminal::GetResponse()
{
	string input = "";
	cin >> input;

	if (input.compare(0, 1, "y") == 0)
		return true;
	else if (input.compare(0, 1, "Y") == 0)
		return true;
	else
		return false;
}

int Terminal::GetNum()
{
	int num = 0;
	cin >> num;
	return num;
}

float Terminal::GetFloat()
{
	float num = 0;
	cin >> num;
	return num;
}

string Terminal::GetString()
{
	string input = "";
	cin >> input;
	return input;
}

void Terminal::HLine(char edge, char c)
{
	cout << edge;
	++pos[0];
	for (int i = 0; i < 78; ++i)
	{
		cout << c;
		++pos[0];
	}
	cout << edge << endl;
	++pos[0];
	++pos[1];
}

void Terminal::Center(char edge, string text)
{
	cout << edge;
	++pos[0];
	int gap = (width - 2 - text.size()) / 2;
	for (int i = 0; i < gap; ++i)
	{
		cout << ' ';
		++pos[0];
	}
	cout << text;
	pos[0] += text.size();
	if (text.size() % 2)
		++gap;
	for (int i = 0; i < gap; ++i)
	{
		cout << ' ';
		++pos[0];
	}
	cout << edge << endl;
	++pos[0];
	++pos[1];
}
