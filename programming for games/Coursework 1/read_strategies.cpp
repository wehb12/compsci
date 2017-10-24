// Will Hinds, Computer Games Engineering MSc - read_strategies.cpp //
// ---------------------------------------------------------------- //
// Reads and executes strategies                                    //

#include "stdafx.h"
#include "strategy.h"
#include "prisoner.h"
#include "BST.h"

using namespace std;

void Game(Prisoner&, Prisoner&);
int Min(int[]);

int main()
{
	int winners[10] = { 0 };
	for (int t = 0; t < 10; ++t)
	{
		int start = 1 + 10 * t;

		cout << endl << "-----------------------" << endl;
		cout << "Tournament" << endl << "-----------------------" << endl;

		int scores[10] = { 0 };

		for (int i = start; i <= start + 9; ++i)
		{
			for (int j = i; j <= start + 9; ++j)
			{
				string id1 = "strat";
				id1 += to_string(i);
				id1 += ".txt";

				string id2 = "strat";
				id2 += to_string(j);
				id2 += ".txt";

				ReadStrategy strat1(id1);
				if (strat1.OpenFile())
				{
					Prisoner A(strat1);
					ReadStrategy strat2(id2);
					if (strat2.OpenFile())
					{
						Prisoner B(strat2);

						for (int k = 0; k < A.GetMaxIterations(); ++k)
							Game(A, B);

						scores[i - start] += A.GetScore();
						scores[j - start] += B.GetScore();
					}
				}
			}
		}

		cout << "The results are:" << endl;
		for (int i = 0; i < 10; ++i)
			cout << i + start << ": " << scores[i] << endl;

		int winner = Min(scores);
		cout << endl << "The victor was Strategy " << winner + start << " with a score of " << scores[winner] << endl;
		winners[t] = winner + start;
	}

	cout << endl << "-----------------------" << endl;
	cout << "Finalist Tournament" << endl << "-----------------------" << endl;

	int scores[10] = { 0 };

	for (int i = 1; i <= 10; ++i)
	{
		for (int j = i; j <= 10; ++j)
		{
			string id1 = "strat";
			id1 += to_string(winners[i-1]);
			id1 += ".txt";

			string id2 = "strat";
			id2 += to_string(winners[j-1]);
			id2 += ".txt";

			ReadStrategy strat1(id1);
			if (strat1.OpenFile())
			{
				Prisoner A(strat1);
				ReadStrategy strat2(id2);
				if (strat2.OpenFile())
				{
					Prisoner B(strat2);

					for (int k = 0; k < A.GetMaxIterations(); ++k)
						Game(A, B);

					scores[i - 1] += A.GetScore();
					scores[j - 1] += B.GetScore();
				}
			}
		}
	}

	cout << "The results are:" << endl;
	for (int i = 0; i < 10; ++i)
		cout << winners[i] << ": " << scores[i] << endl;

	int winner = Min(scores);
	cout << endl << "The overall victor was Strategy " << winners[winner] << " with a score of " << scores[winner] << endl;

	return 0;
}

int Min(int scores[])
{
	int min = 0;
	for (int i = 0; i < 10; ++i)
	{
		if (scores[i] < scores[min])
			min = i;
	}
	return min;
}

void Game(Prisoner& A, Prisoner& B)
{
	int AOutcome = A.Run();
	int BOutcome = B.Run();

	if (AOutcome >= 0)
	{
		if (BOutcome >= 0)
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
		{
			cout << "Error in prisoner B strategy: ";
			if (BOutcome == -1)
				cout << "end of file reached with no exit condition." << endl;
			else if (BOutcome == -2)
				cout << "unknown keyword." << endl;
			else if (BOutcome == -3)
				cout << "infinite loop detected." << endl;
			else
				cout << "unknown error." << endl;
		}
	}
	else
	{
		cout << "Error in prisoner A strategy: ";
		if (AOutcome == -1)
			cout << "end of file reached with no exit condition." << endl;
		else if (AOutcome == -2)
			cout << "unknown keyword." << endl;
		else if (AOutcome == -3)
			cout << "infinite loop detected." << endl;
		else
			cout << "unknown error." << endl;
	};
}
