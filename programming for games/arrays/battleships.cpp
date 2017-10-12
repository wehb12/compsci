// battleships.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"

using namespace std;

char** init(const int size)
{
	char** field = new char*[size];
	for (int i = 0; i < size; i++)
	{
		field[i] = new char[size];
		memset(field[i], '0', size * sizeof(bool));
	}

	return field;
}

void printField(char** field, const int size)
{
	cout << ' ' << ' ' << ' ' << 'A' << ' ' << 'B' << ' ' << 'C' << ' ' << 'D' << ' ' << 'E' << endl;
	for (int i = 0; i < size; i++)
	{
		cout << i << "  ";
		for (int j = 0; j < size; j++)
			cout << field[i][j] << ' ';
		cout << endl;
	}
}

int main()
{
	const int size = 5;

	char** field = init(size);
	char** guess = init(size);

	field[1][1] = '1';
	field[1][2] = '1';
	field[1][3] = '1';
	field[1][4] = '1';
	
	cout << "Your Guesses:" << endl << endl;
	printField(guess, size);

	int x, y;
	cout << endl << "Please enter an X coord: ";
	cin >> x;
	cout << endl << "Please enter an Y coord: ";
	cin >> y;

	if (field[x][y] == 1)
	{
		guess[x][y] = 'X';
		cout << endl << "HIT!" << endl << endl;
	}
	else
	{
		guess[x][y] = 'O';
		cout << endl << "MISS!" << endl << endl;
	}

	printField(guess, size);

    return 0;
}
