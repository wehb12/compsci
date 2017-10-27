// Will Hinds, Computer Games Engineering MSc - gang.h //
// --------------------------------------------------- //
// Contains Gang class with 5 prisoner members         //
// Student ID: 170740805, Date: 27/10/17 10:00         //

#pragma once

#include "prisoner.h"

// Gang class includes a vector of 5 prisoners
// instantiate with 5 file ids for prisoners' strategies
class Gang
{
public:
	Gang(string one, string two, string three, string four, string five);
	Gang();
	Gang(Gang& cpy) { }
	~Gang() { delete members; }
	void SetPrisonerStrats(string one, string two, string three, string four, string five) throw (invalid_argument);
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
	inline int GetAllS() { return allOutcomes[7]; }
	inline int GetAllT() { return allOutcomes[8]; }
	inline int GetAllU() { return allOutcomes[9]; }
	inline int GetAllV() { return allOutcomes[10]; }
	inline void SetSpy() { spy = (rand() % 5) + 1; }
	inline void SetChangeFreq(float perc) { changeFreq = perc; }
private:
	inline void ResetSpy() { spy = 0; }
	inline void SetLeader() { leader = (rand() % 5) + 1; }
	inline bool IsSpy(int guess) { return (guess == spy); }
	inline bool ChangeChoice() { return ((changeFreq * 100) > (rand() % 100)); }
	char lastOutcome;
	// [W] [X] [Y] [Z] [A] [B] [C] [Spy1] [Spy2] [OtherSpy] [BothSpies]
	int allOutcomes[11];
	int iterations;
	double gangScore;
	const int maxIterations = 200;
	int spy;
	int leader;
	float changeFreq;

	vector<Prisoner>* members;
};

Gang::Gang(string one, string two, string three, string four, string five)
{
	lastOutcome = '\0';
	for (int i = 0; i < 11; ++i)
		allOutcomes[i] = 0;
	iterations = 0;
	gangScore = 0;
	spy = 0;
	leader = 0;
	changeFreq = 0;

	SetPrisonerStrats(one, two, three, four, five);
}

Gang::Gang()
{
	lastOutcome = '\0';
	for (int i = 0; i < 11; ++i)
		allOutcomes[i] = 0;
	iterations = 0;
	gangScore = 0;
	spy = 0;
	leader = 0;
	changeFreq = 0;
}

void Gang::SetPrisonerStrats(string one, string two, string three, string four, string five) throw (invalid_argument)
{
	try
	{
		ReadStrategy strat1(one);
		strat1.OpenFile();
		ReadStrategy strat2(two);
		strat2.OpenFile();
		ReadStrategy strat3(three);
		strat3.OpenFile();
		ReadStrategy strat4(four);
		strat4.OpenFile();
		ReadStrategy strat5(five);
		strat5.OpenFile();

		members = new vector<Prisoner>(5);

		(*members)[0].SetStrat(strat1);
		(*members)[1].SetStrat(strat2);
		(*members)[2].SetStrat(strat3);
		(*members)[3].SetStrat(strat4);
		(*members)[4].SetStrat(strat5);
	}
	catch (const invalid_argument& iae)
	{
		throw invalid_argument(iae.what());
	}
}

// returns 0 for all BETRAY and 1 for all SILENCE and -ve number for ERROR
// otherwise return number of people who BETRAYED plus one
// so return 2 means 1 BETRAY, 4 SILENCE
// return 3 means 2 BETRAY, 3 SILENT etc.
//
// returns 6 for spy caught on first guess
// returns 7 for spy caught on second guess
int Gang::Run()
{
	// silences tracks how many members stayed silent
	int silences = 0;

	for (int i = 0; i < 5; ++i)
	{
		int outcome = (*members)[i].Run();
		if (outcome >= 0)
			silences += outcome;
		else return outcome;
	}

	if (spy != 0)
	{
		SetLeader();
		// leader guesses 1 to 5
		int guess = (rand() % 5) + 1;
		if (IsSpy(guess))
			return 6;

		int remove = guess;
		// remove a member that isn't spy or guess
		while ((remove == guess) || IsSpy(remove))
			remove = (rand() % 5) + 1;

		// if leader changes choice
		if (ChangeChoice())
		{
			int lastGuess = guess;
			guess == remove;
			// choose a member that isn't removed or last guess
			while ((guess == remove) || (guess == lastGuess))
				guess = (rand() % 5) + 1;

			if (IsSpy(guess))
				return 7;
		}

		// spy always chooses the minority team
		if (silences > 2)
			++silences;
		else if (silences < 3)
			--silences;
	}

	if (silences > 0)
		silences = 6 - silences;

	return silences;
}

void Gang::RegisterOutcome(char outcome)
{
	// set spy to zero
	ResetSpy();
	lastOutcome = outcome;
	for (int i = 0; i < 5; ++i)
		(*members)[i].RegisterOutcome(outcome);
	
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
		// this gang caught the spy first time
		case 'S':
			++allOutcomes[7];
			break;
		// this gang caught the spy second time
		case 'T':
			++allOutcomes[8];
			gangScore += 2;
			break;
		// the other gang caught the spy
		case 'U':
			++allOutcomes[9];
			gangScore += 5;
			break;
		// both gangs caught the spy
		case 'V':
			++allOutcomes[10];
			gangScore += 6;
			break;
	}
}
