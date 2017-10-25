// prisoners_dilemma.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "generate.h"
#include "ui.h"
#include "prisoner.h"

using namespace std;

vector<string>* Play(vector<string>&) throw (invalid_argument);
void Game(Prisoner&, Prisoner&);
int Min(vector<int>&);

int main()
{
	Terminal t;

	bool prisonerMenu = true;

	while (prisonerMenu)
	{
		t.ClearScreen();
		t.TitleBox("Prisoner's Dilemma");

		t.AddText("Would you like to play the Prisoner's Dilemma? (Y/N)");

		bool prisoners = t.GetResponse();

		prisonerMenu = prisoners;

		while (prisoners)
		{
			t.AddText("Would you like to generate strategies? (Y/N)");

			bool generate = t.GetResponse();

			while (!generate)
			{
				t.AddText("Do you have your own strategies? (Y/N)");

				generate = t.GetResponse();

				if (!generate)
					break;

				t.AddText("Strategies must be numbered sequentially thus: XXXX1.txt, XXXX2.txt etc.");
				t.AddText("Are the strategies formatted correctly? (Y/N)");

				bool correct = t.GetResponse();

				int num = 0;
				vector<string> ids(0);

				if (correct)
				{
					t.AddText("Please enter the prefix text (XXXX)");

					string prefix = t.GetString();

					t.AddText("How many strategies would you like to test?");

					num = t.GetNum();

					for (int i = 1; i <= num; ++i)
					{
						string id = prefix;
						id += to_string(i);
						id += ".txt";
						ids.push_back(id);
					}
				}
				else
				{
					t.AddText("Would you like to manually enter some strategy names? (Y/N)");
					bool enter = t.GetResponse();

					if (!enter)
						break;

					t.AddText("Please enter some strategy names, type \"exit\" when finished");

					string userInput = "";
					while (userInput.compare("exit") != 0)
					{
						userInput = t.GetString();
						if (userInput.compare("exit") == 0)
							break;
						if (userInput.compare(userInput.size() - 4, 4, ".txt") != 0)
							if (userInput.compare(userInput.size() - 4, 4, ".TXT") != 0)
								userInput += ".txt";
						ids.push_back(userInput);
						++num;
					}
				}

				bool play = false;

				if (num > 0)
					play = true;
				else
					break;

				if (play)
				{
					try
					{
						vector<string>* result = Play(ids);
						delete result;
					}
					catch (const invalid_argument& iae)
					{
						string error = "Error: ";
						error += iae.what();
						t.AddText(error);
						t.AddText("Continue?... (Y/N)");
						bool cont = t.GetResponse();
						if (!cont)
							return 1;
					}
					generate = false;
					break;
				}
			}

			while (generate)
			{
				t.AddText("How many strategies would you like to generate? (num)");

				int num = 0;
				num = t.GetNum();

				bool play = false;

				if (num > 0)
					play = true;

				generate = play;

				while (play)
				{
					t.AddText("How long should they be? (number between 0.1 and 1)");

					float length = 0;
					length = t.GetFloat();

					if (length > 0.1)
					{
						Generate(num, length);
						string buff = to_string(num);
						buff += " strategies generated, Play a tournament? (Y/N)";
						t.AddText(buff);

						bool tournament = t.GetResponse();

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

							try
							{
								vector<string>* result = Play(ids);
								delete result;
							}
							catch (const invalid_argument& iae)
							{
								string error = "Error: ";
								error += iae.what();
								t.AddText(error);
								tournament = false;
								t.AddText("Continue?... (Y/N)");
								bool cont = t.GetResponse();
								if (!cont)
									return 1;
							}
							tournament = false;
						}

						play = tournament;
						generate = tournament;
					}
					else
						t.AddText("Please enter a higher number");
				}
			}

			prisoners = generate;
		}
	}

	bool gangMenu = true;

	while (gangMenu)
	{
		t.ClearScreen();
		t.TitleBox("Prisoner's Gang Dilemma");

		t.AddText("Would you like to play the Prisoner's Gang Dilemma? (Y/N)");

		bool gang = t.GetResponse();

		while(gang)
		{

		}

		gangMenu = gang;
	}

	return 0;
}

vector<string>* Play(vector<string>& strats) throw (invalid_argument)
{
	Terminal t;
	t.TitleBox("Tournament");

	int self = 1;
	bool playSelf = true;
	if (strats.size() > 1)
	{
		t.AddText("Would you like the strategies to play themselves? (Y/N)");
		playSelf = t.GetResponse();

		if (playSelf)
			self = 0;
	}

	string text = "There are " + to_string(strats.size()) + " strategies.";
	t.AddText(text);
	t.AddText("Would you like them all in one tournament? (Y/N)");

	int tourns = 1;
	bool chooseTourns = t.GetResponse();
	while (!chooseTourns)
	{
		t.AddText("How many tournaments should they be split into?");
		tourns = t.GetNum();

		int size = strats.size();
		if (!playSelf)
			size = size / 2;
		if (tourns > size)
		{
			text = "Please enter a number " + to_string(size) + " or less";
			t.AddText(text);
		}
		else
			chooseTourns = true;
	}

	t.AddText("Would you like to display a strategy's score after the tournament? (Y/N)");
	bool dispRounds = t.GetResponse();

	t.AddText("Would you like to display the stats of each head-to-head game? (Y/N)");
	bool dispStats = t.GetResponse();

	if ((strats.size() > 10) && dispStats)
	{
		t.AddText("Are you sure (that's a lot of matches) (Y/N)");
		dispStats = t.GetResponse();
	}

	int tournSize = strats.size() / tourns;
	int leftOver = strats.size() - (tournSize * tourns);

	vector<string>* winners = new vector<string>;
	vector<int> winnerScores(0);
	for (int k = 0; k < tourns; ++k)
	{
		int max = tournSize;
		int start = 1 + (k * tournSize);

		if (k == (tourns - leftOver))
		{
			if (leftOver == 0)
				break;
			max = tournSize + 1;
			start += strats.size() - (tournSize * tourns) - leftOver;
			--leftOver;
		}

		vector<int> scores(max);

		t.HLine('+', '-');
		text = "Tournament " + to_string(k + 1);
		t.AddText(text);
		t.HLine('+', '-');

		int game = 1;

		for (int i = start; i < start + max; ++i)
		{
			for (int j = i + self; j < start + max; ++j)
			{
				ReadStrategy strat1(strats[i - 1]);

				vector<vector<string> > statsA;
				vector<vector<string> > statsB;
				vector<string> titles;
				titles.push_back("Score");
				titles.push_back("ALL_W");
				titles.push_back("ALL_X");
				titles.push_back("ALL_Y");
				titles.push_back("ALL_Z");
				statsA.push_back(titles);
				statsB.push_back(titles);

				try
				{
					if (strat1.OpenFile())
					{
						Prisoner A(strat1);
						ReadStrategy strat2(strats[j - 1]);
						if (strat2.OpenFile())
						{
							Prisoner B(strat2);

							for (int k = 0; k < A.GetMaxIterations(); ++k)
								Game(A, B);

							scores[i - start] += A.GetScore();
							scores[j - start] += B.GetScore();

							if (dispStats)
							{
								text = "Game " + to_string(game);
								t.AddText(text);

								vector<string> tableLineA;
								tableLineA.push_back(to_string(A.GetScore()));
								tableLineA.push_back(to_string(A.GetAllW()));
								tableLineA.push_back(to_string(A.GetAllX()));
								tableLineA.push_back(to_string(A.GetAllY()));
								tableLineA.push_back(to_string(A.GetAllZ()));
								statsA.push_back(tableLineA);

								vector<string> tableLineB;
								tableLineB.push_back(to_string(B.GetScore()));
								tableLineB.push_back(to_string(B.GetAllW()));
								tableLineB.push_back(to_string(B.GetAllX()));
								tableLineB.push_back(to_string(B.GetAllY()));
								tableLineB.push_back(to_string(B.GetAllZ()));
								statsB.push_back(tableLineB);

								text = "Strategy " + strats[i - 1] + ":";
								t.AddText(text);
								t.DrawTable(&statsA);
								text = "Strategy " + strats[j - 1] + ":";
								t.AddText(text);
								t.DrawTable(&statsB);
							}
						}
					}
				}
				catch (const invalid_argument& iae)
				{
					delete winners;
					throw invalid_argument(iae.what());
				}
				++game;
			}
			if (dispRounds)
			{
				string finalScore = "Strategy " + strats[i - 1] + " scored " + to_string(scores[i - start]) + " in total";
				t.AddText(finalScore);
			}
		}

		int winner = Min(scores);
		winners->push_back(strats[winner + start - 1]);
		winnerScores.push_back(scores[winner]);
	}

	t.HLine('+', '=');
	t.AddText("Tournament(s) over, the victors were: ");

	int i = 0;
	for (auto it = winners->begin(); it != winners->end(); ++it)
	{
		text = "Tournament " + to_string(i + 1);
		t.AddText(text);
		text = "Strategy " + *it + " with " + to_string(winnerScores[i]);
		++i;
		t.AddText(text);
	}

	t.AddText("Continue...");
	char exit;
	cin >> exit;

	return winners;
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

int Min(vector<int>& vec)
{
	int min = 0;
	for (int i = 0; i < vec.size(); ++i)
	{
		if (vec[i] != 0)
			if (vec[i] < vec[min])
				min = i;
	}
	return min;
}
