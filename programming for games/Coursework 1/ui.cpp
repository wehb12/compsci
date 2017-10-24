// Will Hinds, Computer Games Engineering MSc - UI.cpp //
// --------------------------------------------------- //
// Contains UI code                                    //

#include <iostream>
#include <string>

using namespace std;

class Terminal
{
public:
	Terminal();
	~Terminal();
	void MoveCursor(int x, int y);
	inline void Home() { cout << HOME; pos [0] = 0; pos[1] = 0; }
	inline void ClearScreen() { cout << CLS; pos [0] = 0; pos[1] = 0; }
	inline void Back() { cout << BACK; --pos[0]; }
	inline void Forward() { cout << FORWARD; ++pos[0]; }
	inline void Up() { cout << UP; --pos[1]; }
	inline void Down() { cout << DOWN; ++pos[1]; }
	void SetColour(unsigned int fore, unsigned int back);
private:
	const string HOME = "\033[H";
	const string CLS = "\033[2J";
	const string BACK = "\033[1C";
	const string FORWARD = "\033[1C";
	const string UP = "\033[1A";
	const string DOWN = "\033[1B";
	// BLACK = 30;
	// RED = 31;
	// GREEN = 32;
	// YELLOW = 33;
	// BLUE = 34;
	// MAGENTA = 35;
	// CYAN = 36;
	// WHITE = 37;
	// DEFAULT = 39;
	int colour[2];
	int pos[2];
};

Terminal::Terminal()
{
	colour[0] = 39;
	colour[1] = 49;
	ClearScreen();
	Home();
	pos[0] = 0;
	pos[1] = 0;
}

Terminal::~Terminal()
{
	SetColour(9, 9);
}

void Terminal::MoveCursor(int x, int y)
{
	pos[0] = x;
	pos[1] = y;

	string buff = "\033[";
	buff += to_string(y);
	buff += ';';
	buff += to_string(x);
	buff += 'H';

	cout << buff;
}

void Terminal::SetColour(unsigned int fore, unsigned int back)
{
	if (((fore < 8) || (fore == 9)) && ((back < 8) || (back == 9)))
	{
		string buff = "\033[";
		buff += (30 + fore);
		buff += ';';
		buff += (40 + back);
		buff += 'm';

		cout << buff;
	}
}

int main()
{
	Terminal t1;

	t1.ClearScreen();
	t1.MoveCursor(10, 10);

	cout << "here";
	t1.Up();
	cout << "up";
	t1.Down();
	t1.Down();
	t1.SetColour(4, 7);
	cout << "down";
	t1.Forward();
	cout << "forward";

	return 0;
}
