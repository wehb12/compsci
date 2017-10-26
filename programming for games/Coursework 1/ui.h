// Will Hinds, Computer Games Engineering MSc - UI.h //
// ------------------------------------------------- //
// Contains UI code                                  //

#pragma once

#include <iostream>
#include <string>

using namespace std;

class Table
{
public:
	Table() { }
	~Table() { }
	void HLine(char edge, char c, int lineWidth = 80);
	void DrawTable(vector<vector<string> >* headings);
	void PrintTable(string fileName, vector<vector<string> >* headings, string title = "");
protected:
	void TableHLine(char edge = '+', char c = '=');
	void DrawColumns();
	void FillColumn(string heading);
	ostream* ostr;
private:
	inline void Forward() { cout << ' '; }
	const int width = 80;
	int numCols;
	int columnWidth;
	int space;
	int tableWidth;
};

class Terminal : public Table
{
public:
	Terminal() : Table() { ostr = &cout; ClearScreen(); }
	~Terminal() { }
	void ClearScreen();
	inline void Forward() { cout << ' '; }
	inline void Down() { cout << endl; }
	void Center(char edge, string text);
	void TitleBox(string title);
	void AddText(string text);
	bool GetResponse();
	int GetNum();
	float GetFloat();
	string GetString();
private:
	const int width = 80;
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

void Table::DrawTable(vector<vector<string> >* headings)
{
	ostr = &cout;

	numCols = headings->begin()->size();
	columnWidth = width / numCols;
	space = (width - (columnWidth * numCols)) / 2;
	tableWidth = width - (space * 2);

	TableHLine();
	for (auto it = headings->begin(); it != headings->end(); ++it)
	{
		DrawColumns();

		for (int i = 0; i < space; ++i)
			*ostr << ' ';

		for (auto it2 = it->begin(); it2 != it->end(); ++it2)
		{
			FillColumn(*it2);

			if (it2 != --it->end())
				cout << ' ';
			else
				cout << '|' << endl;
		}

		DrawColumns();

		if (it == --headings->end())
			TableHLine();
		else
			TableHLine('+', '-');
	}
	cout << endl;
}

void Table::PrintTable(string fileName, vector<vector<string> >* headings, string title)
{
	ofstream file(fileName);
	ostr = &file;

	if (title.size() != 0)
		*ostr << title << endl;

	numCols = headings->begin()->size();
	columnWidth = width / numCols;
	space = (width - (columnWidth * numCols)) / 2;
	tableWidth = width - (space * 2);

	TableHLine();
	for (auto it = headings->begin(); it != headings->end(); ++it)
	{
		DrawColumns();

		for (auto it2 = it->begin(); it2 != it->end(); ++it2)
		{
			FillColumn(*it2);

			if (it2 != --it->end())
				*ostr << ' ';
			else
				*ostr << '|' << endl;
		}

		DrawColumns();

		if (it == --headings->end())
			TableHLine();
		else
			TableHLine('+', '-');
	}
	*ostr << endl;

	ostr = &cout;
}

void Table::TableHLine(char edge, char c)
{
	for (int i = 0; i < space; ++i)
		Forward();

	HLine(edge, c, tableWidth);
}

void Table::DrawColumns()
{
	int adjust = 1;
	for (int i = 0; i < space; ++i)
		*ostr << ' ';
	for (int j = 0; j < numCols; ++j)
	{
		*ostr << '|';
		if (j == (numCols - 1))
			adjust = 2;
		for (int i = 0; i < columnWidth - adjust; ++i)
			*ostr << ' ';
	}
	*ostr << '|';
	*ostr << endl;
}

void Table::FillColumn(string heading)
{
	*ostr << '|';
	int gap = -1;
	if (heading.size() <= (columnWidth - 1))
		gap = (columnWidth - 1 - heading.size()) / 2;
	else if (heading.size() > (columnWidth - 2))
		heading.assign(heading, 0, (columnWidth - 2));
	for (int i = 0; i < gap; ++i)
		*ostr << ' ';
	*ostr << heading;
	if ((columnWidth - 1 - heading.size()) % 2)
		++gap;
	for (int i = 0; i < gap - 1; ++i)
		*ostr << ' ';
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

void Table::HLine(char edge, char c, int lineWidth)
{
	*ostr << edge;
	for (int i = 0; i < (lineWidth - 2); ++i)
		*ostr << c;
	*ostr << edge << endl;
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
