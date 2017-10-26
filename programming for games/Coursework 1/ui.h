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
	Terminal() { ClearScreen(); }
	~Terminal() { }
	void ClearScreen();
	inline void Forward() { cout << ' '; }
	inline void Down() { cout << endl; }
	void HLine(char edge, char c, int lineWidth = 80);
	void Center(char edge, string text);
	void TitleBox(string title);
	void AddText(string text);
	void DrawTable(vector<vector<string> >* headings);
	bool GetResponse();
	int GetNum();
	float GetFloat();
	string GetString();
private:
	void TableHLine(int space, int tableWidth, char edge = '+', char c = '=');
	void DrawColumns(int columnWidth, int numCols);
	void FillColumn(int columnWidth, string heading);
	const int width = 80;
	const int height = 80;
};

#ifdef _WIN32
void Terminal::ClearScreen()
{
	system("cls");
}
#else
void Terminal::ClearScreen()
{
	cout << "\033[2J";
}
#endif

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

void Terminal::DrawTable(vector<vector<string> >* headings)
{
	int numCols = headings->begin()->size();
	int columnWidth = width / numCols;
	int space = (width - (columnWidth * numCols)) / 2;
	int tableWidth = width - (space * 2);

	TableHLine(space, tableWidth);
	for (auto it = headings->begin(); it != headings->end(); ++it)
	{
		DrawColumns(columnWidth, numCols);

		for (auto it2 = it->begin(); it2 != it->end(); ++it2)
		{
			FillColumn(columnWidth, *it2);

			if (it2 != --it->end())
				cout << ' ';
			else
				cout << '|' << endl;
		}

		DrawColumns(columnWidth, numCols);

		if (it == --headings->end())
			TableHLine(space, tableWidth);
		else
			TableHLine(space, tableWidth, '+', '-');
	}
	cout << endl;
}

void Terminal::TableHLine(int space, int tableWidth, char edge, char c)
{
	for (int i = 0; i < space; ++i)
		Forward();

	HLine(edge, c, tableWidth);
}

void Terminal::DrawColumns(int columnWidth, int numCols)
{
	int adjust = 1;
	for (int j = 0; j < numCols; ++j)
	{
		cout << '|';
		if (j == (numCols - 1))
			adjust = 2;
		for (int i = 0; i < columnWidth - adjust; ++i)
			cout << ' ';
	}
	cout << '|';
	cout << endl;
}

void Terminal::FillColumn(int columnWidth, string heading)
{
	cout << '|';
	int gap = -1;
	if (heading.size() <= (columnWidth - 2))
		gap = (columnWidth - 2 - heading.size()) / 2;
	else if (heading.size() > (columnWidth - 2))
		heading.assign(heading, 0, (columnWidth - 2));
	for (int i = 0; i <= gap; ++i)
		cout << ' ';
	cout << heading;
	if (heading.size() % 2)
		++gap;
	for (int i = 0; i < gap - 1; ++i)
		cout << ' ';
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

void Terminal::HLine(char edge, char c, int lineWidth)
{
	cout << edge;
	for (int i = 0; i < (lineWidth - 2); ++i)
		cout << c;
	cout << edge << endl;
}

void Terminal::Center(char edge, string text)
{
	cout << edge;

	int gap = (width - 2 - text.size()) / 2;
	for (int i = 0; i < gap; ++i)
		cout << ' ';
	cout << text;

	if (text.size() % 2)
		++gap;
	for (int i = 0; i < gap; ++i)
		cout << ' ';
	cout << edge << endl;
}
