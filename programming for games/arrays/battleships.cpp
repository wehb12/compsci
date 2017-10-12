// battleships.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"

using namespace std;

class Battleships
{
public:
	Battleships();
	~Battleships();
	void PrintGuesses();
	void RegisterGuess(int x, int y);
	bool CheckScore();
private:
	const int size = 5;
	char **field;
	char **guess;
	int hitPoints;
	int* score;
};

Battleships::Battleships()
{
	field = new char*[size];
	guess = new char*[size];
	for (int i = 0; i < size; i++)
	{
		field[i] = new char[size];
		guess[i] = new char[size];
		memset(field[i], '0', size * sizeof(char));
		memset(guess[i], '_', size * sizeof(char));
	}

	field[1][1] = '1';
	field[1][2] = '1';
	field[1][3] = '1';
	field[1][4] = '1';

	hitPoints = 4;

	score = new int;
	*score = 0;
}

Battleships::~Battleships()
{
	for (int i = 0; i < size; i++)
	{
		delete[] field[i];
		delete[] guess[i];
	}
	delete field;
	delete guess;
}

void Battleships::PrintGuesses()
{
	cout << ' ' << ' ' << ' ' << '0' << ' ' << '1' << ' ' << '2' << ' ' << '3' << ' ' << '4' << endl;
	for (int i = 0; i < size; i++)
	{
		cout << i << "  ";
		for (int j = 0; j < size; j++)
			cout << guess[i][j] << ' ';
		cout << endl;
	}
}

void Battleships::RegisterGuess(int x, int y)
{
	if (field[y][x] == '1')
	{
		guess[y][x] = 'X';
		*score = *score + 1;
		cout << endl << "HIT!" << endl;
	}
	else
	{
		guess[y][x] = 'O';
		cout << endl << "MISS!" << endl;
	}
}

bool Battleships::CheckScore()
{
	if (*score == hitPoints)
		return true;
	else return false;
}

int main()
{
	Battleships game1;

	while (1)
	{
		cout << "Your Guesses:" << endl << endl;
		game1.PrintGuesses();

		int x = 0, y = 0;
		cout << endl << "Please enter an X coord: ";
		cin >> x;
		cout << endl << "Please enter an Y coord: ";
		cin >> y;

		game1.RegisterGuess(x, y);
		if (game1.CheckScore())
		{
			cout << endl << "You sunk my battleship!!" << endl;
			cout << "END OF GAME";
			return 0;
		}
		
		char cont = NULL;
		while (!((cont == 'Y') || (cont == 'y')))
		{
			cout << endl << "Continue? (Y/N)... ";
			cin >> cont;

			if ((cont == 'N') || (cont == 'n'))
				return 0;
			else if (!((cont == 'Y') || (cont == 'y')))
				cout << endl << "Please enter 'Y' or 'N'... ";
		}
		system("CLS");
	}
}
