// generate_strategies.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>

using namespace std;

int main()
{
	int strats = 0;			// number of startegies created
	float length = 0.5;		// number that dictates length - not length in lines, probability of it being longer
	const int lengthAdjust = 1;	// the higher this number, the lower the number of lines
	int line = 1;			// which line are we on?
	int seed = 0;

	++strats;

	seed = 100;//rand() % 500;
	if (seed == 0)
		seed = 1;

	string id = "strat";
	id += to_string(strats);
	id += ".txt";

	ofstream strat(id);

	strat << "This strategy had a seed of " << seed << ", a length adjust variable of " << lengthAdjust
		<< " and a user set length adjuster of " << length << endl << endl;

	while (line < (((float)seed * length) / lengthAdjust))
	{
		strat << line << ' ';

		float exp = line;
		float maxExp = log(2100000000) / log(seed);
		while (exp > maxExp)
			exp -= maxExp;

		int randNum = pow(seed, exp);
		randNum = randNum % 72;

		if (randNum >= 36)	// print an exit condition
		{
			if (randNum <= 47)
				strat << "BETRAY" << endl;
			else if (randNum <= 59)
				strat << "SILENCE" << endl;
			else
				strat << "RANDOM" << endl;
		}
		else				// print out a statement
		{
			strat << "IF ";
			if (randNum >= 24)
			{
				strat << "LASTOUTCOME = ";
				if (randNum <= 2)
					strat << "W ";
				else if (randNum <= 5)
					strat << "X ";
				else if (randNum <= 8)
					strat << "Y ";
				else
					strat << "Z ";
			}
			else
			{
				bool exitFlag = false;
				bool secondHalf = false;
				int tempNum = randNum;

				while (!exitFlag)
				{
					float maxExp = log(2100000000) / log(tempNum);
					while ((exp > maxExp) && tempNum)
						exp -= maxExp;

					int temp = pow(tempNum, exp);
					temp = temp % 200;

					if (tempNum <= 11)
						strat << temp << ' ';
					else
					{
						strat << "ALLOUTCOMES_";
						if (tempNum <= 14)
							strat << "W ";
						else if (tempNum <= 17)
							strat << "X ";
						else if (tempNum <= 20)
							strat << "Y ";
						else
							strat << "Z ";
					}

					if (temp >= 80)
					{
						if (!secondHalf)
						{
							if (temp <= 119)
								strat << "= ";
							else if (temp <= 159)
								strat << "< ";
							else
								strat << "> ";
							secondHalf = true;
						}
						else
							exitFlag = true;
					}
					else
					{
						if (temp <= 39)
							strat << "+ ";
						else
							strat << "- ";
					}

					exp++;
					tempNum++;
					tempNum = pow(tempNum, exp);
					tempNum = tempNum % 24;
				}
			}

			strat << "GOTO " << (line + 2) << endl;
			if (line >= (((float)seed * length) / lengthAdjust) - 1)
			{
				for (int i = 0; i < 2; ++i)
				{
					randNum = pow(seed, line);
					randNum = randNum % 36;

					if (randNum <= 11)
						strat << "BETRAY" << endl;
					else if (randNum <= 23)
						strat << "SILENCE" << endl;
					else
						strat << "RANDOM" << endl;
					++line;
				}
				--line;
			}
		}
		++line;
	}
    return 0;
}
