#pragma once

#include "stdafx.h"
#include "strategy.h"

using namespace std;

class Prisoner
{
public:
	Prisoner(Strategy& str) { Init(str); }
	Prisoner() {  }
	Prisoner(Prisoner & cpy) {  }

	int Run();
	void RegisterOutcome(char outcome);
private:
	void Init(Strategy& str);
	Strategy strat;
	char lastOutcome;
	int allOutcomes[4];
	int iterations;
	int myScore;

	int Enumerate(string buff);
	void AssignVariable(bool& var, bool& sym, bool& minus, bool secondHalf, int lhs[], int rhs[], int i);
	char AssignOutcome(bool& expectOutcome, bool& expectGoto, const char last);
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

// returns 0 for BETRAY and 1 for SILENCE and -1 for ERROR
int Prisoner::Run()
{
	unsigned int line = 0;
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
		LINENO,
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
	bool jump = false;
	int lhs[6] = { 0, 0, 0, 0, 0, 0 };
	int rhs[6] = { 0, 0, 0, 0, 0, 0 };
	char ifLast = '\0';
	bool secondHalf = false;
	bool minus;

	while (1)
	{
		string buff = strat.GetInstr(line, word);

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
			break;
		case GOTO:
			if (expectGoto)
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
			if (expectVariable)
			{
				expectVariable = false;
				expectSymbol = true;
				expectCompare = true;
				expectOutcome = true;
			}
			break;
		case W:
			if (expectOutcome && !expectCompare)
				ifLast = AssignOutcome(expectOutcome, expectGoto, 'W');
			break;
		case X:
			if (expectOutcome && !expectCompare)
				ifLast = AssignOutcome(expectOutcome, expectGoto, 'X');
			break;
		case Y:
			if (expectOutcome && !expectCompare)
				ifLast = AssignOutcome(expectOutcome, expectGoto, 'Y');
			break;
		case Z:
			if (expectOutcome && !expectCompare)
				ifLast = AssignOutcome(expectOutcome, expectGoto, 'Z');
			break;
		case LINENO:
			if (expectLineNo)
			{
				expectLineNo = false;

			}
			break;
		case ERROR:
			return -1;
			break;
		}
	}
}

char Prisoner::AssignOutcome(bool& expectOutcome, bool& expectGoto, const char last)
{
	expectOutcome = false;
	expectGoto = true;
	return last;
}

void Prisoner::AssignVariable(bool& var, bool& sym, bool& minus, bool secondHalf, int lhs[], int rhs[], int i)
{
	sym = true;
	var = false;
	int sign = 1;
	if (minus)
	{
		sign = -1;
		minus = false;
	}
	if (secondHalf)
		rhs[i] = sign * allOutcomes[2];
	else
		lhs[i] = sign * allOutcomes[2];
}

int Prisoner::Enumerate(string buff)
{
	if (buff.compare("BETRAY") == 0) return 0;
	if (buff.compare("SILENCE") == 0) return 1;
	if (buff.compare("RANDOM") == 0) return 2;
	if (buff.compare("IF") == 0) return 3;
	if (buff.compare("GOTO") == 0) return 4;
	if (buff.compare(0, 12, "ALLOUTCOMES_"))
	{
		if (buff.compare(12, 1, "W")) return 5;
		if (buff.compare(12, 1, "X")) return 6;
		if (buff.compare(12, 1, "Y")) return 7;
		if (buff.compare(12, 1, "Z")) return 8;
	}
	if (buff.compare("ITERATIONS") == 0) return 9;
	if (buff.compare("MYSCORE") == 0) return 10;
	if (buff.compare("LASTOUTCOME") == 0) return 11;
	if (buff.compare("W")) return 12;
	if (buff.compare("X")) return 13;
	if (buff.compare("Y")) return 14;
	if (buff.compare("Z")) return 15;
	if ((buff.compare("9") <= 0) || (buff.compare("0") >= 0)) return 17;
	if (buff.compare("+") == 0) return 18;
	if (buff.compare("-") == 0) return 19;
	if (buff.compare("=") == 0) return 20;
	if (buff.compare(">") == 0) return 21;
	if (buff.compare("<") == 0) return 22;
	if (buff.compare("EOL") == 0) return 23;
	if (buff.compare("EOF") == 0) return 24;
	return 25;
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