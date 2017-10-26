#pragma once

#include "strategy.h"
#include "BST.h"
#include "comparable.h"

using namespace std;

class Prisoner
{
public:
	Prisoner(Strategy& str) { Init(str); }
	Prisoner() {  }
	Prisoner(Prisoner& cpy) {  }

	int Run();
	void RegisterOutcome(char outcome);
	inline int GetMaxIterations() { return maxIterations; }
	inline int GetScore() { return myScore; }
	inline int GetAllW() { return allOutcomes[0]; }
	inline int GetAllX() { return allOutcomes[1]; }
	inline int GetAllY() { return allOutcomes[2]; }
	inline int GetAllZ() { return allOutcomes[3]; }
	inline void SetStrat(Strategy& str) { strat = str; }
	Prisoner& operator=(Prisoner& rhs);
private:
	void Init(Strategy& str);
	Strategy strat;
	char lastOutcome;
	// [W] [X] [Y] [Z] [A] [B] [C]
	int allOutcomes[7];
	int iterations;
	int myScore;
	const int maxIterations = 200;

	bool CheckForInfLoops(BST<Integer>& tree, int leaf);
	int Enumerate(string buff);
	void AssignVariable(bool& var, bool& sym, bool& minus, bool secondHalf, int lhs[], int rhs[], int i);
	char AssignOutcome(bool& expectOutcome, bool& expectGoto, bool& expectLast, bool& expectCompare, bool& expectSymbol, const char last);
	int Sum(int arr[]);
};

void Prisoner::Init(Strategy& str)
{
	strat = str;
	lastOutcome = '\0';
	for (int i = 0; i < 4; ++i)
		allOutcomes[i] = 0;
	iterations = 0;
	myScore = 0;
}

// returns 0 for BETRAY and 1 for SILENCE and -ve number for ERROR
int Prisoner::Run()
{
	if (iterations == maxIterations)
		return -1;

	unsigned int line = 0;
	unsigned int fileLine = strat.GetLineNo(line);
	if (fileLine == -1)
		return -4;
	unsigned int word = 1;

	enum keywords
	{
		BETRAY = 0,
		SILENCE,
		RANDOM,
		IF,
		GOTO,
		ALL_W,
		ALL_X,
		ALL_Y,
		ALL_Z,
		ITERATIONS,
		MYSCORE,
		LAST,
		W,
		X,
		Y,
		Z,
		NUMBER,
		PLUS,
		SUBTRACT,
		EQUALS,
		GREATER,
		LESS,
		EOL,
		EOFile,
		ERROR
	};

	bool expectVariable = false;
	bool expectGoto = false;
	bool expectLineNo = false;
	bool expectSymbol = false;
	bool expectCompare = false;
	bool expectOutcome = false;
	bool expectLast = false;
	// [0] - ALL_W, [1] - ALL_X, [2] - ALL_Y, [3] - ALL_Z,
	// [4] - ITERATIONS, [5] - MYSCORE, [6] - INTEGER
	int lhs[7] = { 0, 0, 0, 0, 0, 0, 0 };
	int rhs[7] = { 0, 0, 0, 0, 0, 0, 0 };
	int compareOperator = 0; // 0 for =, 1 for >, 2 for <
	char ifLast = '\0';
	bool secondHalf = false;
	bool minus = false;

	BST<Integer> lineTree(line);

	while (1)
	{
		string buff = strat.GetInstr(fileLine, word);

		switch (Enumerate(buff))
		{
		case BETRAY:
			return 0;
			break;
		case SILENCE:
			return 1;
			break;
		case RANDOM:
			return rand() % 2;
			break;
		case IF:
			expectVariable = true;
			expectLast = true;
			expectOutcome = true;
			break;
		case GOTO:
			if (expectGoto || (expectSymbol && secondHalf))
				expectLineNo = true;
			break;
		case ALL_W:
			if (expectVariable)
				AssignVariable(expectVariable, expectSymbol, minus, secondHalf, lhs, rhs, 0);
			break;
		case ALL_X:
			if (expectVariable)
				AssignVariable(expectVariable, expectSymbol, minus, secondHalf, lhs, rhs, 1);
			break;
		case ALL_Y:
			if (expectVariable)
				AssignVariable(expectVariable, expectSymbol, minus, secondHalf, lhs, rhs, 2);
			break;
		case ALL_Z:
			if (expectVariable)
				AssignVariable(expectVariable, expectSymbol, minus, secondHalf, lhs, rhs, 3);
			break;
		case ITERATIONS:
			if (expectVariable)
				AssignVariable(expectVariable, expectSymbol, minus, secondHalf, lhs, rhs, 4);
			break;
		case MYSCORE:
			if (expectVariable)
				AssignVariable(expectVariable, expectSymbol, minus, secondHalf, lhs, rhs, 5);
			break;
		case LAST:
			if (expectLast && !expectCompare)
			{
				expectVariable = false;
				expectSymbol = true;
				expectCompare = true;
				if (!expectOutcome)
					expectGoto = true;
				else
					expectOutcome = true;
				expectLast = false;
			}
			break;
		case W:
			if (expectOutcome && !expectCompare)
				ifLast = AssignOutcome(expectOutcome, expectGoto, expectLast, expectCompare, expectSymbol, 'W');
			break;
		case X:
			if (expectOutcome && !expectCompare)
				ifLast = AssignOutcome(expectOutcome, expectGoto, expectLast, expectCompare, expectSymbol, 'X');
			break;
		case Y:
			if (expectOutcome && !expectCompare)
				ifLast = AssignOutcome(expectOutcome, expectGoto, expectLast, expectCompare, expectSymbol, 'Y');
			break;
		case Z:
			if (expectOutcome && !expectCompare)
				ifLast = AssignOutcome(expectOutcome, expectGoto, expectLast, expectCompare, expectSymbol, 'Z');
			break;
		case NUMBER:
			if (expectLineNo)
			{
				expectLineNo = false;
				if (ifLast != '\0')
				{
					if (lastOutcome == ifLast)
					{
						fileLine = stoi(buff);
						line = strat.GetActualLine(fileLine);
					}
					else
					{
						++line;
						fileLine = strat.GetLineNo(line);
					}
				}
				else
				{
					switch (compareOperator)
					{
					case 0:
						if (Sum(lhs) == Sum(rhs))
						{
							fileLine = stoi(buff);
							line = strat.GetActualLine(fileLine);
						}
						else
						{
							++line;
							fileLine = strat.GetLineNo(line);
						}
						break;
					case 1:
						if (Sum(lhs) > Sum(rhs))
						{
							fileLine = stoi(buff);
							line = strat.GetActualLine(fileLine);
						}
						else
						{
							++line;
							fileLine = strat.GetLineNo(line);
						}
						break;
					case 2:
						if (Sum(lhs) < Sum(rhs))
						{
							fileLine = stoi(buff);
							line = strat.GetActualLine(fileLine);
						}
						else
						{
							++line;
							fileLine = strat.GetLineNo(line);
						}
						break;
					}
				}
				word = 0;
				if (CheckForInfLoops(lineTree, line))
					return -3;

				expectVariable = false;
				expectGoto = false;
				expectLineNo = false;
				expectSymbol = false;
				expectCompare = false;
				expectOutcome = false;
				expectLast = false;
				memset(lhs, 0, 7 * sizeof(int));
				memset(rhs, 0, 7 * sizeof(int));
				compareOperator = 0;
				ifLast = '\0';
				secondHalf = false;
				minus = false;
			}
			else if (expectVariable)
			{
				expectVariable = false;
				expectSymbol = true;

				int sign = 1;
				if (minus)
				{
					sign = -1;
					minus = false;
				}
				if (secondHalf)
					rhs[6] += sign * stoi(buff);
				else
					lhs[6] += sign * stoi(buff);
			}
			break;
		case PLUS:
			if (expectSymbol)
			{
				expectSymbol = false;
				expectVariable = true;
			}
			break;
		case SUBTRACT:
			if (expectSymbol)
			{
				expectSymbol = false;
				expectVariable = true;
				minus = true;
			}
			break;
		case EQUALS:
			if (expectSymbol)
			{
				expectSymbol = false;
				expectCompare = false;
				expectVariable = true;
				secondHalf = true;
				compareOperator = 0;
			}
			break;
		case GREATER:
			if (expectSymbol)
			{
				expectSymbol = false;
				expectCompare = false;
				expectVariable = true;
				secondHalf = true;
				compareOperator = 1;
			}
			break;
		case LESS:
			if (expectSymbol)
			{
				expectSymbol = false;
				expectCompare = false;
				expectVariable = true;
				secondHalf = true;
				compareOperator = 2;
			}
			break;
		case EOL:
			++line;
			if (CheckForInfLoops(lineTree, line))
				return -3;
			word = 0;
			break;
		case EOF:
			return -1;
			break;
		case ERROR:
			return -2;
			break;
		}
		++word;
	}
}

bool Prisoner::CheckForInfLoops(BST<Integer>& tree, int leaf)
{
	if (tree.Find(tree.GetRoot(), leaf))
		return true;
	else
		tree.Insert(tree.GetRoot(), leaf);
	return false;
}

int Prisoner::Sum(int arr[])
{
	int sum = 0;
	for (int i = 0; i < 7; ++i)
		sum += arr[i];
	return sum;
}

char Prisoner::AssignOutcome(bool& expectOutcome, bool& expectGoto, bool& expectLast, bool& expectCompare, bool& expectSymbol, const char last)
{
	expectOutcome = false;
	if (!expectLast)
		expectGoto = true;
	else
	{
		expectSymbol = true;
		expectCompare = true;
	}
	return last;
}

void Prisoner::AssignVariable(bool& var, bool& sym, bool& minus, bool secondHalf, int lhs[], int rhs[], int i)
{
	int sum = 0;
	if (i < 4)
		sum = allOutcomes[i];
	else if (i == 4)
		sum = iterations;
	else if (i == 5)
		sum = myScore;

	sym = true;
	var = false;
	int sign = 1;
	if (minus)
	{
		sign = -1;
		minus = false;
	}
	if (secondHalf)
		rhs[i] += sign * sum;
	else
		lhs[i] += sign * sum;
}

int Prisoner::Enumerate(string buff)
{
	if (buff.compare("BETRAY") == 0) return 0;
	if (buff.compare("SILENCE") == 0) return 1;
	if (buff.compare("RANDOM") == 0) return 2;
	if (buff.compare("IF") == 0) return 3;
	if (buff.compare("GOTO") == 0) return 4;
	if (buff.compare(0, 12, "ALLOUTCOMES_") == 0)
	{
		if (buff.compare(12, 1, "W") == 0) return 5;
		if (buff.compare(12, 1, "X") == 0) return 6;
		if (buff.compare(12, 1, "Y") == 0) return 7;
		if (buff.compare(12, 1, "Z") == 0) return 8;
	}
	if (buff.compare("ITERATIONS") == 0) return 9;
	if (buff.compare("MYSCORE") == 0) return 10;
	if (buff.compare("LASTOUTCOME") == 0) return 11;
	if (buff.compare("W") == 0) return 12;
	if (buff.compare("X") == 0) return 13;
	if (buff.compare("Y") == 0) return 14;
	if (buff.compare("Z") == 0) return 15;
	int first = 0;
	if (buff.compare(0, 1, "-") == 0)
		first = 1;
	if ((buff.compare(first, 1, "9") <= 0) && (buff.compare(first, 1, "0") >= 0)) return 16;
	if (buff.compare("+") == 0) return 17;
	if (buff.compare("-") == 0) return 18;
	if (buff.compare("=") == 0) return 19;
	if (buff.compare(">") == 0) return 20;
	if (buff.compare("<") == 0) return 21;
	if (buff.compare("EOL") == 0) return 22;
	if (buff.compare("EOF") == 0) return 23;
	return 24;
}

void Prisoner::RegisterOutcome(char outcome)
{
	++iterations;
	lastOutcome = outcome;
	switch (outcome)
	{
	case 'W':
		++allOutcomes[0];
		myScore += 2;
		break;
	case 'X':
		++allOutcomes[1];
		myScore += 5;
		break;
	case 'Y':
		++allOutcomes[2];
		break;
	case 'Z':
		++allOutcomes[3];
		myScore += 4;
		break;
	}
}

Prisoner& Prisoner::operator=(Prisoner& rhs)
{
	if (this == &rhs)
		return *this;

	strat = rhs.strat;
	lastOutcome = rhs.lastOutcome;
	for (int i = 0; i < 4; ++i)
		allOutcomes[i] = rhs.allOutcomes[i];
	iterations = rhs.iterations;
	myScore = rhs.myScore;
}
