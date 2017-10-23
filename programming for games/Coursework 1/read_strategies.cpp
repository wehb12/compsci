// Will Hinds, Computer Games Engineering MSc - read_strategies.cpp //
// ---------------------------------------------------------------- //
// Reads and executes strategies                                    //

#include "strategy.h"

using namespace std;

int main()
{
	int strats = 1;

	string id = "Desktop/Uni/Masters/Programming_for_Games/Coursework_1/strat";
	id += to_string(strats);
	id += ".txt";

	ReadStrategy strat(id);

	string out = "Desktop/Uni/Masters/Programming_for_Games/Coursework_1/read_strat.txt";

	ofstream output(out);
	output << strat;
}