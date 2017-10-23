// Will Hinds, Computer Games Engineering MSc - read_strategies.cpp //
// ---------------------------------------------------------------- //
// Reads and executes strategies                                    //

#include "stdafx.h"
#include "strategy.h"
#include "prisoner.h"

using namespace std;

void Game(Prisoner&, Prisoner&);

int main()
{
	string id1 = "strat";
	id1 += to_string(1);
	id1 += ".txt";

	string id2 = "strat";
	id2 += to_string(2);
	id2 += ".txt";

	ReadStrategy strat1(id1);
	if (strat1.OpenFile())
	{
		Prisoner A(strat1);
		ReadStrategy strat2(id2);
		if (strat2.OpenFile())
		{
			Prisoner B(strat2);

			for (int i = 0; i < 5; ++i)
				Game(A, B);
		}
	}

	return 0;
}

void Game(Prisoner& A, Prisoner& B)
{
	int AOutcome = A.Run();
	int BOutcome = B.Run();

	if (AOutcome != -1)
	{
		if (BOutcome != -1)
		{
			if (AOutcome == 0)
			{
				if (BOutcome == 0)
				{
					A.RegisterOutcome('Z');
					B.RegisterOutcome('Z');
				}
				else
				{
					A.RegisterOutcome('Y');
					B.RegisterOutcome('X');
				}
			}
			else
			{
				if (BOutcome == 0)
				{
					A.RegisterOutcome('X');
					B.RegisterOutcome('Y');
				}
				else
				{
					A.RegisterOutcome('W');
					B.RegisterOutcome('W');
				}
			}
		}
		else
			cout << "Error in prisoner B strategy" << endl;
	}
	else
		cout << "Error in prisoner A strategy" << endl;
}
