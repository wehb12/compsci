// Will Hinds, Computer Games Engineering MSc - strategy.h //
// ------------------------------------------------------- //
// Contains class definitions for Strategy classes         //

#pragma once

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include "comparable.h"

using namespace std;

class Strategy : public Comparable
{
public:
	inline Strategy(const string txt) : name(txt), strat(new vector<vector<string> >(1)) { }
	inline Strategy() : name("default"), strat(new vector<vector<string> >(1)) { }
	inline Strategy(const Strategy& cpy) : name(cpy.name), strat(new vector<vector<string> >(1)) { }
	~Strategy() { delete strat; }

	friend ostream& operator<<(ostream& ostr, const Strategy& str);
	Strategy& operator=(const Strategy& rhs);
	inline string GetName() { return name; }
	virtual void NewLine();
	string GetInstr(unsigned int line, unsigned int word);
	virtual int CompareTo(const Comparable& rhs) override;
protected:
	string name;
	vector<vector<string> >* strat;
};

class ReadStrategy : public Strategy
{
public:
	inline ReadStrategy(const string txt) : Strategy(txt) { }
	inline ReadStrategy() : Strategy() { }
	inline ReadStrategy(const ReadStrategy& cpy) : Strategy(cpy) { }
	~ReadStrategy() { }

	bool OpenFile();
	inline void AddFeature(string txt) { strat->back().push_back(txt); }
private:
	void ReadFile(ifstream& file);
};

class CreateStrategy : public ReadStrategy
{
public:
	inline CreateStrategy(const string txt) : ReadStrategy(txt) { ClearChecksLine(); }
	inline CreateStrategy() : ReadStrategy() { ClearChecksLine(); }
	inline CreateStrategy(const CreateStrategy& cpy) : ReadStrategy(cpy) { ClearChecksLine(); }
	~CreateStrategy() { }

	void AddFeature(int num);
	using ReadStrategy::AddFeature;
	void NewLine();
	void SetFlag(int flagNum);
	inline bool GetFlag(int flagNum) { return checks[flagNum]; }
	void PrintStrategy();
protected:
	// boolean checks to ensure a strategy that makes sense is constructed, with no variable repeats
	// values ares set to true if their variable has been printed before
	// [0] - ALLOUTCOMES_W, [1] - ALLOUTCOMES_X, [2] - ALLOUTCOMES_Y, [3] - ALLOUTCOMES_Z,
	// [4] - INTEGER, [5] - ITERATIONS, [6] - MYSCORE, [7] - =, < OR >
	// [8] - Only one variable left, so if =, < OR > hasn't been, then print one
	// [9] - All variables used, so terminate line
	bool checks[10];
	void ClearChecksLine();
	void ComputeSum();
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

Strategy& Strategy::operator=(const Strategy& rhs)
{
	if (this == &rhs)
		return *this;

	delete strat;
	strat = new vector<vector<string> >;

	for (auto it = rhs.strat->begin(); it != rhs.strat->end(); ++it)
	{
		NewLine();

		for (auto it2 = it->begin(); it2 != it->end(); it2++)
			strat->back().push_back(*it2);
	}

	name = rhs.name;

	return *this;
}

void Strategy::NewLine()
{
	vector<string> temp(0);
	strat->push_back(temp);
}

string Strategy::GetInstr(unsigned int line, unsigned int word)
{
	if (strat->size() > line)
	{
		if ((*strat)[line].size() > word)
			return (*strat)[line][word];
		else
			return "EOL";
	}
	else
		return "EOF";
}

int Strategy::CompareTo(const Comparable& rhs)
{
	const Strategy *s = dynamic_cast<const Strategy*>(&rhs);

	int thisLines = strat->size();
	int sLines = s->strat->size();

	if (thisLines < sLines)
		return -1;
	else if (thisLines > sLines)
		return 1;
	else	// same number of lines in each strategy
	{
		auto sIt = s->strat->begin();
		for (auto it = strat->begin(); it != strat->end(); ++it)
		{
			if (it->size() < sIt->size())
				return -1;
			else if (it->size() > sIt->size())
				return 1;
			else
			{
				auto sIt2 = sIt->begin();
				for (auto it2 = it->begin(); it2 != it->end(); ++it2)
				{
					if ((*it2).compare(*sIt2) < 0)
						return -1;
					else if ((*it2).compare(*sIt2) > 0)
						return 1;
					++sIt2;
				}
			}
			++sIt;
		}
	}
	return 0;
}

bool ReadStrategy::OpenFile()
{
	ifstream file;

	file.open(name);

	if (!file.is_open())
	{
		cout << "Unable to open file " << name << endl;
		return false;
	}

	ReadFile(file);

	file.close();

	return true;
}

void ReadStrategy::ReadFile(ifstream& file)
{
	bool space = false;		// set to true if a space was read last
	while (!file.eof())
	{
		string line = "";
		string word = "";

		getline(file, line);

		for (unsigned int i = 0; i < line.size(); ++i)
		{
			if (line[i] == ' ')
			{
				if (!space)
				{
					AddFeature(word);
					word = "";
				}
				space = true;
			}
			else
			{
				word += line[i];
				space = false;
			}
		}
		AddFeature(word);
		NewLine();
	}
}

void CreateStrategy::SetFlag(int flagNum)
{
	checks[flagNum] = true;
	ComputeSum();
}

void CreateStrategy::ComputeSum()
{
	int sum = 0;
	for (int i = 0; i < 5; ++i)
		if (checks[i])
			++sum;
	if (sum == 6)
		checks[8] = true;
	else if (sum == 7)
		checks[9] = true;
}

void CreateStrategy::ClearChecksLine()
{
	for (int i = 0; i < 10; ++i)
		checks[i] = false;
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
	ClearChecksLine();
}

void CreateStrategy::PrintStrategy()
{
	ofstream file(name);
	file << *this;
}
