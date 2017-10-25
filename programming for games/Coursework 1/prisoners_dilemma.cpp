// prisoners_dilemma.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "stdafx.h"
#include "generate.h"
#include "read.h"
#include "ui.h"

using namespace std;

vector<int>* Play(vector<string>&);
void Game(Prisoner&, Prisoner&);
int Min(int[]);

int main()
{
	Terminal t1;

	bool prisonerMenu = true;

	while (prisonerMenu)
	{
		t1.ClearScreen();
		t1.TitleBox("Prisoner's Dilemma");

		t1.AddText("Would you like to play the Prisoner's Dilemma? (Y/N)");

		bool prisoners = t1.GetResponse();

		prisonerMenu = prisoners;

		while (prisoners)
		{
			t1.AddText("Would you like to generate strategies? (Y/N)");

			bool generate = t1.GetResponse();

			while (!generate)
			{
				t1.AddText("Do you have your own strategies? (Y/N)");

				generate = t1.GetResponse();

				if (!generate)
					break;

				t1.AddText("Strategies must be numbered sequentially thus: XXXX1.txt, XXXX2.txt etc.");
				t1.AddText("If formatted correctly, please enter the prefix text (XXXX)");
				t1.AddText("If they are not, please enter \"exit\" and generate some strategies.");

				string prefix = t1.GetString();

				if (prefix.compare("exit") == 0)
					break;

				t1.AddText("How many strategies would you like to test?");

				int num = t1.GetNum();

				bool play = false;

				if (num > 0)
					play = true;
				else
					break;

				if (play)
				{
					vector<string> ids(0);
					for (int i = 1; i <= num; ++i)
					{
						string id = prefix;
						id += to_string(i);
						id += ".txt";
						ids.push_back(id);
					}
					vector<int>* result = Play(ids);
					generate = false;
					break;
				}

			}

			while (generate)
			{
				t1.AddText("How many strategies would you like to generate? (num)");

				int num = 0;
				num = t1.GetNum();

				bool play = false;

				if (num > 0)
					play = true;

				generate = play;

				while (play)
				{
					t1.AddText("How long should they be? (number between 0.1 and 1)");

					float length = 0;
					length = t1.GetFloat();

					if (length > 0.1)
					{
						Generate(num, length);
						string buff = to_string(num);
						buff += " strategies generated, Play a tournament? (Y/N)";
						t1.AddText(buff);

						bool tournament = t1.GetResponse();

						if (tournament)
						{
							vector<string> ids(0);
							for (int i = 1; i <= num; ++i)
							{
								string id = "strat";
								id += to_string(i);
								id += ".txt";
								ids.push_back(id);
							}
							vector<int>* result = Play(ids);
						}

						play = tournament;
						generate = tournament;
					}
					else
						t1.AddText("Please enter a higher number");
				}
			}

			prisoners = generate;
		}
	}

	t1.AddText("Would you like to play the Prisoner's Gang Dilemma? (Y/N)");

	bool gang = t1.GetResponse();

	while (gang)
	{
		t1.ClearScreen();
		t1.TitleBox("Prisoner's Gang Dilemma");
		gang = !gang;
	}

    return 0;
}

vector<int>* Play(vector<string>& strats)
{
	int tens = strats.size() / 10;
	int leftOver = strats.size() % 10;
	vector<int> winners(0);
	for (int t = 0; t <= tens; ++t)
	{
		int scores[10] = { 0 };
		int max = 10;
		int start = 1 + (t * 10);

		if (t == tens)
		{
			if (leftOver == 0)
				break;
			max = leftOver;
		}
		for (int i = start; i < start + max; ++i)
		{
			for (int j = i; j < start + max; ++j)
			{
				ReadStrategy strat1(strats[i-1]);
				if (strat1.OpenFile())
				{
					Prisoner A(strat1);
					ReadStrategy strat2(strats[j-1]);
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
		int winner = Min(scores);
		winners.push_back(winner + start);
	}
	return &winners;
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

int Min(int arr[])
{
	int min = 0;
	for (int i = 0; i < 10; ++i)
	{
		if (arr[i] != 0)
			if (arr[i] < arr[min])
				min = i;
	}
	return min;
}