// generate_strategies.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include "strategy.h"
#include "BST.h"

using namespace std;

bool Symbol(CreateStrategy&, int, float);
bool AllOutcomes(CreateStrategy&, int);
bool Integer(CreateStrategy&, int, float);
void ExitCondition(CreateStrategy&, int);
void LastOutcome(CreateStrategy&, int);
int PsuedoRand(int, float&);

int main()
{
	const float length = 0.00001;	// number that dictates length - not length in lines, probability of it being longer

	BST<Strategy> stratTree;

	int attempts = 0;

	for (int strats = 1; strats <= 10; ++strats)
	{
		int line = 0;			// which line of the strategy are we on?
		int seed = 0;
		bool exitLast = false;	// variable that stops generation if two exit keywords printed in a row
		bool goTo = false;

		seed = rand() % 1000;
		if (seed == 0)
			seed = 1;

		string id = "strat";
		id += to_string(strats);
		id += ".txt";

		CreateStrategy strat(id);

		while (line < ((float)seed * length))
		{
			++line;
			strat.AddFeature(line);

			float exp = line;
			int randNum = PsuedoRand(seed, exp);
			randNum = randNum % 72;

			if (randNum >= 36)	// print an exit condition
			{
				ExitCondition(strat, randNum);

				if (exitLast || !goTo)
					break;

				exitLast = true;
			}
			else				// print out a statement
			{
				exitLast = false;

				strat.AddFeature("IF");
				if (randNum >= 24)
					LastOutcome(strat, randNum);
				else
				{
					bool exitFlag = false;
					bool symbol = false;	// tells program a symbol is wanted next

					while (!exitFlag)
					{
						if (randNum <= 1)
							symbol = Integer(strat, seed, exp);
						else
							symbol = AllOutcomes(strat, randNum);

						if (symbol || (!strat.GetFlag(7) && strat.GetFlag(8)) || strat.GetFlag(9))
						{
							exitFlag = Symbol(strat, randNum, exp);
							symbol = false;
						}

						randNum++;
						exp++;

						randNum = PsuedoRand(randNum, exp);
						randNum = randNum % 24;
					}
				}

				goTo = true;
				strat.AddFeature("GOTO");
				strat.AddFeature(line + 2);
				strat.NewLine();
				if (line >= ((float)seed * length) - 1)
				{
					int lastNum = -1;
					for (int i = 0; i < 2; ++i)
					{
						randNum = pow(seed, line);
						randNum = randNum % 3;

						while (randNum == lastNum)
						{
							++seed;
							randNum = pow(seed, line);
							randNum = randNum % 3;
						}

						lastNum = randNum;
						++line;

						strat.AddFeature(line);
						ExitCondition(strat, randNum);
					}
					--line;
				}
			}
		}

		if (strats == 1)
		{
			stratTree.SetRoot(strat);
			strat.PrintStrategy();
		}
		else
		{
			if (!stratTree.Find(stratTree.GetRoot(), strat))
			{
				stratTree.Insert(stratTree.GetRoot(), strat);
				strat.PrintStrategy();
			}
			else
				--strats;
		}

		++attempts;
		if (attempts > 99)
		{
			cout << "Reached limit of attempts to create " << 10 << " unique strategies" << endl;
			return 1;
		}
	}

	return 0;
}

bool Symbol(CreateStrategy& strat, int num, float exp)
{
	int temp = PsuedoRand(num, exp);
	temp = temp % 200;

	if ((temp >= 50) || (!strat.GetFlag(7) && strat.GetFlag(8)) || strat.GetFlag(9))
	{
		if (!strat.GetFlag(7))
		{
			if (temp <= 99)
				strat.AddFeature("=");
			else if (temp <= 149)
				strat.AddFeature("<");
			else
				strat.AddFeature(">");
			strat.SetFlag(7);
		}
		else
			return true;
	}
	else
	{
		if (temp <= 19)
			strat.AddFeature("+");
		else
			strat.AddFeature("-");
	}
	return false;
}

bool AllOutcomes(CreateStrategy& strat, int num)
{
	num = num % 6;
	switch (num)
	{
	case 0:
		if (!strat.GetFlag(0))
		{
			strat.AddFeature("ALLOUTCOMES_W");
			strat.SetFlag(0);
			return true;
		}
		break;
	case 1:
		if (!strat.GetFlag(1))
		{
			strat.AddFeature("ALLOUTCOMES_X");
			strat.SetFlag(1);
			return true;
		}
		break;
	case 2:
		if (!strat.GetFlag(2))
		{
			strat.AddFeature("ALLOUTCOMES_Y");
			strat.SetFlag(2);
			return true;
		}
		break;
	case 3:
		if (!strat.GetFlag(3))
		{
			strat.AddFeature("ALLOUTCOMES_Z");
			strat.SetFlag(3);
			return true;
		}
		break;
	case 4:
		if (!strat.GetFlag(5))
		{
			strat.AddFeature("ITERATIONS");
			strat.SetFlag(5);
			return true;
		}
		break;
	case 5:
		if (!strat.GetFlag(6))
		{
			strat.AddFeature("MYSCORE");
			strat.SetFlag(6);
			return true;
		}
		break;
	}
	return false;
}

bool Integer(CreateStrategy& strat, int num, float exp)
{
	int temp = PsuedoRand(num, exp);
	temp = temp % 200;

	if (!strat.GetFlag(4))
	{
		strat.AddFeature(temp);
		strat.SetFlag(4);
		return true;
	}
	return false;
}

void ExitCondition(CreateStrategy& strat, int num)
{
	num = num % 3;

	switch (num)
	{
	case 0:
		strat.AddFeature("BETRAY");
		break;
	case 1:
		strat.AddFeature("SILENCE");
		break;
	case 2:
		strat.AddFeature("RANDOM");
		break;
	}
	strat.NewLine();
}

void LastOutcome(CreateStrategy& strat, int num)
{
	num = num % 4;
	strat.AddFeature("LASTOUTCOME");
	strat.AddFeature("=");
	switch (num)
	{
	case 0:
		strat.AddFeature("W");
		break;
	case 1:
		strat.AddFeature("X");
		break;
	case 2:
		strat.AddFeature("Y");
		break;
	case 3:
		strat.AddFeature("Z");
		break;
	}
}

int PsuedoRand(int base, float& exp)
{
	if (base == 0)
		base = 1;
	float maxExp = log(2100000000) / log(base);
	while (exp > abs(maxExp))
		exp -= maxExp;

	return (int)pow(base, exp);
}
