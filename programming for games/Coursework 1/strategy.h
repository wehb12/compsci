// Will Hinds, Computer Games Engineering MSc - strategy.h //
// ------------------------------------------------------- //
// Contains class definitions for Strategy classes         //

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

class Strategy
{
public:
	inline Strategy(const string txt) : name(txt) { strat = new vector<vector<string> >(1); };
	inline Strategy() : name("default") { strat = new vector<vector<string> >(1);  }
	inline Strategy(const Strategy& cpy) : name("default") { strat = new vector<vector<string> >(1); }
	~Strategy() { delete strat; }

	friend ostream& operator<<(ostream& ostr, const Strategy& str);
	inline string GetName() { return name; }
protected:
	string name;
	vector<vector<string> >* strat;
};

class ReadStrategy : public Strategy
{
public:
	inline ReadStrategy(const string txt) : Strategy(txt) { OpenStrategy(); }
	inline ReadStrategy() : Strategy() { OpenStrategy(); }
	inline ReadStrategy(const ReadStrategy& cpy) : Strategy(cpy) { OpenStrategy(); }
	~ReadStrategy() { }

	inline void AddFeature(string txt) { strat->back().push_back(txt); }
	virtual void NewLine();
private:
	void OpenStrategy();
	void ReadFile(ifstream& file);
	bool open;
};

class CreateStrategy : public ReadStrategy
{
public:
	inline CreateStrategy(const string txt) : ReadStrategy(txt) { ClearChecksLine(); }
	inline CreateStrategy() : ReadStrategy() { ClearChecksLine(); }
	inline CreateStrategy(const CreateStrategy& cpy) : ReadStrategy(cpy) { ClearChecksLine();  }
	~CreateStrategy() { }

	void AddFeature(int num);
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

void ReadStrategy::NewLine()
{
	vector<string> temp(0);
	strat->push_back(temp);
}

void ReadStrategy::OpenStrategy()
{
	ifstream file;

	file.open(name);

	if (!file.is_open())
	{
		cout << "Unable to open file " << name << endl;
		return;
	}

	ReadFile(file);

	file.close();
}

void ReadStrategy::ReadFile(ifstream& file)
{
	while (!file.eof())
	{
		string line = "";
		string word = "";

		getline(file, line);

		for (int i = 0; i < line.size(); ++i)
		{
			if (line[i] == ' ')
			{
				AddFeature(word);
				word = "";
			}
			else
				word += line[i];
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
