// generate_strategies.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

int PsuedoRand(int, float&);

class Strategy
{
public:
	inline Strategy(const string txt) : name(txt) { strat = new vector<vector<string>>(1); };
	inline Strategy() : name("default") { strat = new vector<vector<string>>(1);  }
	inline Strategy(const Strategy& cpy) : name("default") { strat = new vector<vector<string>>(1); }
	~Strategy() { delete strat; }
	friend ostream& operator<<(ostream& ostr, const Strategy& str);
	inline string GetName() { return name; }
protected:
	string name;
	vector<vector<string>>* strat;
};

ostream& operator<<(ostream& ostr, const Strategy& str)
{
	for (auto it = str.strat->begin(); it != str.strat->end(); ++it)
	{
		for (auto it2 = it->begin(); it2 != it->end(); it2++)
			ostr << *it2 << ' ';
		ostr << endl;
	}

	return ostr;
}

class CreateStrategy : public Strategy
{
public:
	inline CreateStrategy(const string txt) : Strategy(txt) { FirstChecksLine(); }
	inline CreateStrategy() : Strategy() { FirstChecksLine(); }
	inline CreateStrategy(const CreateStrategy& cpy) : Strategy(cpy) { FirstChecksLine();  }
	~CreateStrategy() { delete checks; }
	inline void AddFeature(string txt) { strat->back().push_back(txt); }
	void AddFeature(int num);
	void NewLine();
	void SetFlag(int flagNum);
	bool GetFlag(int flagNum);
protected:
	// boolean checks to ensure a strategy that makes sense is constructed, with no variable repeats
	// values ares set to true if their variable has been printed before
	// [0] - ALLOUTCOMES_W, [1] - ALLOUTCOMES_X, [2] - ALLOUTCOMES_Y, [3] - ALLOUTCOMES_Z,
	// [4] - INTEGER, [5] - ITERATIONS, [6] - MYSCORE, [7] - =, < OR >
	// [8] - Only one variable left, so if =, < OR > hasn't been, then print one
	// [9] - All variables used, so terminate line
	vector<vector<char>>* checks;
	void FirstChecksLine();
	void NewChecksLine();
	void ComputeSum();
};

void CreateStrategy::SetFlag(int flagNum)
{
	checks->back()[flagNum] = true;
	ComputeSum();
}

bool CreateStrategy::GetFlag(int flagNum)
{
	return checks->back()[flagNum];
}

void CreateStrategy::ComputeSum()
{
	int sum = 0;
	for (int i = 0; i < 5; ++i)
		if (checks->back()[i])
			++sum;
	if (sum == 4)		// change these numbers when you add in flags [5] and [6]
		checks->back()[8] = true;
	else if (sum == 5)
		checks->back()[9] = true;
}

void CreateStrategy::FirstChecksLine()
{
	checks = new vector<vector<char>>(0);
	NewChecksLine();
}

void CreateStrategy::NewChecksLine()
{
	vector<char> temp;
	checks->push_back(temp);

	for (int i = 0; i < 10; ++i)
		checks->back().push_back(false);
}

void CreateStrategy::AddFeature(int num)
{
	string txt = to_string(num);
	strat->back().push_back(txt);
}

void CreateStrategy::NewLine()
{
	vector<string> temp(0);
	strat->push_back(temp);
	NewChecksLine();
}

int main()
{
	int strats = 0;			// number of startegies created
	float length = 0.05;	// number that dictates length - not length in lines, probability of it being longer
	int line = 1;			// which line of the strategy are we on?
	int seed = 0;

	++strats;

	seed = 247;//rand() % 500;
	if (seed == 0)
		seed = 1;

	string id = "strat";
	id += to_string(strats);
	id += ".txt";

	CreateStrategy strat1(id);

	while (line < ((float)seed * length))
	{
		strat1.AddFeature(line);

		float exp = line;
		int randNum = PsuedoRand(seed, exp);
		randNum = randNum % 72;

		if (randNum >= 36)	// print an exit condition
		{
			if (randNum <= 47)
				strat1.AddFeature("BETRAY");
			else if (randNum <= 59)
				strat1.AddFeature("SILENCE");
			else
				strat1.AddFeature("RANDOM");
			strat1.NewLine();
		}
		else				// print out a statement
		{
			strat1.AddFeature("IF");
			if (randNum >= 24)
			{
				strat1.AddFeature("LASTOUTCOME");
				strat1.AddFeature("=");
				if (randNum <= 2)
					strat1.AddFeature("W");
				else if (randNum <= 5)
					strat1.AddFeature("X");
				else if (randNum <= 8)
					strat1.AddFeature("Y");
				else
					strat1.AddFeature("Z");
			}
			else
			{
				bool exitFlag = false;
				bool symbol = false;	// tells program a symbol is wanted next
				int tempNum = randNum;

				while (!exitFlag)
				{
					int temp = PsuedoRand(tempNum, exp);
					temp = temp % 200;

					if (tempNum <= 6)
					{
						if (!strat1.GetFlag(4))
						{
							strat1.AddFeature(temp);
							strat1.SetFlag(4);
							symbol = true;
						}
					}
					else
					{
						if ((tempNum <= 14) && !strat1.GetFlag(0))
						{
							strat1.AddFeature("ALLOUTCOMES_W");
							strat1.SetFlag(0);
							symbol = true;
						}
						else if ((tempNum <= 17) && !strat1.GetFlag(1))
						{
							strat1.AddFeature("ALLOUTCOMES_X");
							strat1.SetFlag(1);
							symbol = true;
						}
						else if ((tempNum <= 20) && !strat1.GetFlag(2))
						{
							strat1.AddFeature("ALLOUTCOMES_Y");
							strat1.SetFlag(2);
							symbol = true;
						}
						else if (!strat1.GetFlag(3))
						{
							strat1.AddFeature("ALLOUTCOMES_Z");
							strat1.SetFlag(3);
							symbol = true;
						}
					}

					if (symbol || (!strat1.GetFlag(7) && strat1.GetFlag(8)) || strat1.GetFlag(9))
					{
						if ((temp >= 40) || (!strat1.GetFlag(7) && strat1.GetFlag(8)) || strat1.GetFlag(9))
						{
							if (!strat1.GetFlag(7))
							{
								if (temp <= 119)
									strat1.AddFeature("=");
								else if (temp <= 159)
									strat1.AddFeature("<");
								else
									strat1.AddFeature(">");
								strat1.SetFlag(7);
							}
							else
								exitFlag = true;
						}
						else
						{
							if (temp <= 19)
								strat1.AddFeature("+");
							else
								strat1.AddFeature("-");
						}
						symbol = false;
					}

					tempNum++;
					exp++;

					tempNum = PsuedoRand(tempNum, exp);
					tempNum = tempNum % 24;
				}
			}

			strat1.AddFeature("GOTO");
			strat1.AddFeature(line + 2);
			strat1.NewLine();
			if (line >= ((float)seed * length) - 1)
			{
				for (int i = 0; i < 2; ++i)
				{
					randNum = pow(seed, line);
					randNum = randNum % 36;

					strat1.AddFeature(line);
					if (randNum <= 11)
						strat1.AddFeature("BETRAY");
					else if (randNum <= 23)
						strat1.AddFeature("SILENCE");
					else
						strat1.AddFeature("RANDOM");
					++line;
					strat1.NewLine();
				}
				--line;
			}
		}
		++line;
	}

	ofstream file(strat1.GetName());

	file << strat1;

    return 0;
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
