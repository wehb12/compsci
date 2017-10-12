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
	cout << ' ' << ' ' << ' ' << '0' << ' ' << '1' << ' ' << '2' << ' ' << '3' << ' ' << '4' << endl;
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

	while (1)
	{
		cout << "Your Guesses:" << endl << endl;
		printField(guess, size);

		int x = 0, y = 0;
		cout << endl << "Please enter an X coord: ";
		cin >> x;
		cout << endl << "Please enter an Y coord: ";
		cin >> y;

		if (field[y][x] == '1')
		{
			guess[y][x] = 'X';
			cout << endl << "HIT!" << endl;
		}
		else
		{
			guess[y][x] = 'O';
			cout << endl << "MISS!" << endl;
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

    return 0;
}
