// generate_strategies.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

class Strategy
{
public:
	inline Strategy(const string txt) : name(txt) { strat = new vector<vector<string>>; }
	inline Strategy() : name("default") { strat = new vector<vector<string>>;  }
	inline Strategy(const Strategy& cpy) : name("default") { strat = new vector<vector<string>>; }
	~Strategy() { delete strat; }
	friend ostream& operator<<(ostream& ostr, const Strategy& str);
	inline string GetName() { return name; }
protected:
	string name;
	vector<vector<string>>* strat;
};

ostream& operator<<(ostream& ostr, const Strategy& str)
{
	int i = 0;
	int j = 0;

	for (auto it = str.strat->begin(); it != str.strat->end(); ++it)
	{
		for (auto it2 = str.strat->begin()->begin(); it2 != str.strat->end()->end(); it2++)
		{
			ostr << it[i][j] << ' ';
			++j;
		}
		ostr << endl;
		++i;
	}
}

class CreateStrategy : public Strategy
{
public:
	inline CreateStrategy(const string txt) : Strategy(txt) { }
	inline CreateStrategy() : Strategy() { }
	inline CreateStrategy(const CreateStrategy& cpy) : Strategy(cpy) { }
	~CreateStrategy() { }
	inline void AddFeature(string txt) { strat->back().push_back(txt); }
	void AddFeature(int num);
	void NewLine();
};

void CreateStrategy::AddFeature(int num)
{
	string txt = "";
	txt += num;
	strat->back().push_back(txt);
}

void CreateStrategy::NewLine()
{
	vector<string> temp(0);
	strat->push_back(temp);
}

int main()
{
	int strats = 0;			// number of startegies created
	float length = 0.5;		// number that dictates length - not length in lines, probability of it being longer
	const int lengthAdjust = 1;	// the higher this number, the lower the number of lines
	int line = 1;			// which line are we on?
	int seed = 0;

	++strats;

	seed = 1000;//rand() % 500;
	if (seed == 0)
		seed = 1;

	string id = "strat";
	id += to_string(strats);
	id += ".txt";

	CreateStrategy strat1(id);

	while (line < (((float)seed * length) / lengthAdjust))
	{
		strat1.AddFeature(line);

		float exp = line;
		float maxExp = log(2100000000) / log(seed);
		while (exp > maxExp)
			exp -= maxExp;

		int randNum = pow(seed, exp);
		randNum = randNum % 72;

		if (randNum >= 36)	// print an exit condition
		{
			if (randNum <= 47)
				strat1.AddFeature("BETRAY");
			else if (randNum <= 59)
				strat1.AddFeature("SILENCE");
			else
				strat1.AddFeature("RANDOM");
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
				bool secondHalf = false;
				int tempNum = randNum;

				while (!exitFlag)
				{
					float maxExp = log(2100000000) / log(tempNum);
					while ((exp > maxExp) && tempNum)
						exp -= maxExp;

					int temp = pow(tempNum, exp);
					temp = temp % 200;

					if (tempNum <= 6)
						strat1.AddFeature(temp);
					else
					{
						if (tempNum <= 14)
							strat1.AddFeature("ALLOUTCOMES_W");
						else if (tempNum <= 17)
							strat1.AddFeature("ALLOUTCOMES_X");
						else if (tempNum <= 20)
							strat1.AddFeature("ALLOUTCOMES_Y");
						else
							strat1.AddFeature("ALLOUTCOMES_Z");
					}

					if (temp >= 80)
					{
						if (!secondHalf)
						{
							if (temp <= 119)
								strat1.AddFeature("=");
							else if (temp <= 159)
								strat1.AddFeature("<");
							else
								strat1.AddFeature(">");
							secondHalf = true;
						}
						else
							exitFlag = true;
					}
					else
					{
						if (temp <= 39)
							strat1.AddFeature("+");
						else
							strat1.AddFeature("-");
					}

					exp++;
					tempNum++;
					maxExp = log(2100000000) / log(tempNum);
					while ((exp > maxExp) && tempNum)
						exp -= maxExp;
					tempNum = pow(tempNum, exp);
					tempNum = tempNum % 24;
				}
			}

			strat1.AddFeature("GOTO");
			strat1.AddFeature(line + 2);
			strat1.NewLine();
			if (line >= (((float)seed * length) / lengthAdjust) - 1)
			{
				for (int i = 0; i < 2; ++i)
				{
					randNum = pow(seed, line);
					randNum = randNum % 36;

					if (randNum <= 11)
						strat1.AddFeature("BETRAY");
					else if (randNum <= 23)
						strat1.AddFeature("SILENCE");
					else
						strat1.AddFeature("RANDOM");
					++line;
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
