#pragma once

#include "prisoner.h"

class Gang
{
public:
	Gang(Strategy& one, Strategy& two, Strategy& three, Strategy& four, Strategy& five);
	~Gang() { }
	int Run();
	void RegisterOutcome(char outcome);
	inline int GetMaxIterations() { return maxIterations; }
	inline double GetScore() { return gangScore; }
	inline int GetAllW() { return allOutcomes[0]; }
	inline int GetAllX() { return allOutcomes[1]; }
	inline int GetAllY() { return allOutcomes[2]; }
	inline int GetAllZ() { return allOutcomes[3]; }
	inline int GetAllA() { return allOutcomes[4]; }
	inline int GetAllB() { return allOutcomes[5]; }
	inline int GetAllC() { return allOutcomes[6]; }
private:
	char lastOutcome;
	// [W] [X] [Y] [Z] [A] [B] [C]
	int allOutcomes[7];
	int iterations;
	double gangScore;
	const int maxIterations = 200;
	int spy;

	Prisoner members[5];
};

Gang::Gang(Strategy& one, Strategy& two, Strategy& three, Strategy& four, Strategy& five)
{
	lastOutcome = '\0';
	for (int i = 0; i < 7; ++i)
		allOutcomes[i] = 0;
	iterations = 0;
	gangScore = 0;
	spy = 0;

	members[0].SetStrat(one);
	members[1].SetStrat(two);
	members[2].SetStrat(three);
	members[3].SetStrat(four);
	members[4].SetStrat(five);
}

// returns 0 for all BETRAY and 1 for all SILENCE and -ve number for ERROR
// otherwise return number of people who BETRAYED plus one
// so return 2 means 1 BETRAY, 4 SILENCE
// return 3 means 2 BETRAY, 3 SILENT etc.
int Gang::Run()
{
	int silences = 0;

	for (int i = 0; i < 5; ++i)
	{
		int outcome = members[i].Run();
		if (outcome >= 0)
			silences += outcome;
		else return outcome;
	}

	if (silences > 0)
		silences = 6 - silences;

	return silences;
}

void Gang::RegisterOutcome(char outcome)
{
	lastOutcome = outcome;
	for (int i = 0; i < 5; ++i)
		members[i].RegisterOutcome(outcome);
	
	switch(outcome)	
	{
		case 'W':
			++allOutcomes[0];
			gangScore += 2;
			break;
		case 'X':
			++allOutcomes[1];
			gangScore += 5;
			break;
		case 'Y':
			++allOutcomes[2];
			break;
		case 'Z':
			++allOutcomes[3];
			gangScore += 4;
			break;
		case 'A':
			++allOutcomes[4];
			gangScore += 2.5;
			break;
		case 'B':
			++allOutcomes[5];
			gangScore += 3;
			break;
		case 'C':
			++allOutcomes[6];
			gangScore += 2;
			break;
	}
}