// prisoners_dilemma.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "generate.h"
#include "ui.h"
#include "prisoner.h"
#include "gang.h"

using namespace std;

vector<string>* Play(vector<string>&) throw (invalid_argument);
vector<string>* GangPlay(vector<string>&, bool spy = false) throw (invalid_argument);
template <typename T>
void Game(T&, T&);

Gang* ConstructSecondGang(vector<string>&, const int, const int, const int, const int, const int, vector<int>&) throw (invalid_argument);
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
			vector<string> ids(0);
			bool play = false;

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

					generate = false;
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

				if (num > 0)
				{
					play = true;
					generate = false;
				}
				break;
			}

			while (generate)
			{
				t.AddText("How many strategies would you like to generate? (num)");

				int num = 0;
				num = t.GetNum();

				bool chooseLength = false;

				if (num > 0)
					chooseLength = true;

				generate = chooseLength;

				while (chooseLength)
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

						play = t.GetResponse();

						if (play)
						{
							for (int i = 1; i <= num; ++i)
							{
								string id = "strat";
								id += to_string(i);
								id += ".txt";
								ids.push_back(id);
							}
						}

						chooseLength = false;
						generate = false;
					}
					else
						t.AddText("Please enter a higher number");
				}
			}

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
					play = false;
					t.AddText("Continue?... (Y/N)");
					bool cont = t.GetResponse();
					if (!cont)
						return 1;
				}
				play = false;
			}

			prisoners = play;
		}
	}

	bool gangMenu = true;

	while (gangMenu)
	{
		t.ClearScreen();
		t.TitleBox("Prisoner's Gang Dilemma");

		t.AddText("Would you like to play the Prisoner's Gang Dilemma? (Y/N)");

		bool gang = t.GetResponse();
		gangMenu = gang;

		while (gang)
		{
			t.AddText("1. You can play 10 unique strategies against each other");
			t.AddText("2. Or test every permutation of 10 strategies against a constant set of 5");
			t.AddText("Would you like Option 1 or Option 2?");

			int option = t.GetNum();
			bool generate = false;
			if ((option == 1) || (option == 2))
				generate = true;
			else
				t.AddText("Please enter either '1' or '2'");

			if (generate)
			{
				t.AddText("Would you like to manually enter some strategy names? (Y/N)");
				bool enter = t.GetResponse();

				int num;
				if (option == 1)
					num = 10;
				else
					num = 15;
				vector<string> ids(0);

				if (!enter)
				{
					Generate(num, 0.5);
					string text = to_string(num) + " strategies have been generated";
					t.AddText(text);

					for (int i = 1; i <= num; ++i)
					{
						string id = "strat";
						id += to_string(i);
						id += ".txt";
						ids.push_back(id);
					}
					enter = true;
				}
				else
				{
					string text = "Please enter " + to_string(num) + " strategy names, or type \"exit\"";
					t.AddText(text);

					ids.clear();
					string userInput = "";
					int userNum = 0;
					while ((userInput.compare("exit") != 0) && (userNum != num))
					{
						userInput = t.GetString();
						if (userInput.compare("exit") == 0)
							break;
						if (userInput.size() >= 4)
						{
							if (userInput.compare(userInput.size() - 4, 4, ".txt") != 0)
								if (userInput.compare(userInput.size() - 4, 4, ".TXT") != 0)
									userInput += ".txt";
						}
						else
							userInput += ".txt";
						ids.push_back(userInput);
						++userNum;
					}
					if (userInput.compare("exit") == 0)
					{
						enter = false;
						generate = false;
						gang = false;
					}
				}

				if (enter)
				{
					try
					{
						vector<string>* result = GangPlay(ids);

						t.AddText("Would you like to replay, with a Spy involved disrupting the best team? (Y/N)");
						bool spy = t.GetResponse();

						if (spy)
						{
							for (int i = 0; i < 5; ++i)
							{
								if (ids.size() == 15)
									result->push_back(ids[10 + i]);
								else
									result->push_back(ids[5 + i]);
							}
							GangPlay(*result, true);
						}

						gang = false;
						delete result;
					}
					catch (const invalid_argument& iae)
					{
						string error = "Error: ";
						error += iae.what();
						t.AddText(error);
						gang = false;
						t.AddText("Continue?... (Y/N)");
						bool cont = t.GetResponse();
						if (!cont)
							return 1;
					}
				}
			}
		}
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

	t.AddText("Would you like to print the stats of each head-to-head game to a file? (Y/N)");
	bool printStats = t.GetResponse();

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

							if (dispStats || printStats)
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

								if (printStats)
								{
									string nameM = "stats" + to_string(i) + ".txt";
									string nameP = "stats" + to_string(j) + ".txt";

									t.PrintTable(nameM, &statsA);
									t.PrintTable(nameP, &statsB);
								}
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

vector<string>* GangPlay(vector<string>& strats, bool spy)
{
	Terminal t;

	float prob = 0;
	if (!spy)
		t.TitleBox("Gang Tournament");
	else
	{
		t.TitleBox("Gang Dilemma With Spy");
		t.AddText("With what probability do you want the leader to change their choice? (0 to 1)");
		prob = t.GetFloat();
	}

	bool control = false;
	if (strats.size() == 15)
		control = true;

	bool permu = true;
	if (!control && !spy)
	{
		int combinations = strats.size();
		int end = combinations - 5;

		for (int i = 1; i < end; ++i)
			combinations = combinations * (strats.size() - i);

		combinations = combinations / 240;
		string text = "For " + to_string(strats.size()) + " strategies there are " + to_string(combinations) + " permutations";
		t.AddText(text);

		t.AddText("Would you like to test every permutation? (Y/N)");
		permu = t.GetResponse();
	}

	int jMax = 2;
	int kMax = 3;
	int mMax = 4;
	int nMax = 5;
	if (permu && !spy)
	{
		jMax = 7;
		kMax = 8;
		mMax = 9;
		nMax = 10;
	}

	t.AddText("Would you like to display the stats of each head-to-head game? (Y/N)");
	bool dispStats = t.GetResponse();

	t.AddText("Would you like to print the stats of each head-to-head game to a file? (Y/N)");
	bool printStats = t.GetResponse();

	vector<vector<int>> allScores;
	vector<string>* winners = new vector<string>;

	int iMax = 1;
	if (control)
		iMax = 6;

	int game = 0;
	
	int spyMax = 1;
	if (spy)
		spyMax = 20;

	for (int spyProb = 0; spyProb <= spyMax; spyProb += 5)
	{
		for (int i = 0; i < iMax; ++i)
		{
			for (int j = i + 1; j < jMax; ++j)
			{
				vector<int> scoreM(6);
				vector<int> scoreP(6);
				scoreM[0] = i;
				scoreM[1] = j;
				for (int k = j + 1; k < kMax; ++k)
				{
					scoreM[2] = k;
					for (int m = k + 1; m < mMax; ++m)
					{
						scoreM[3] = m;
						for (int n = m + 1; n < nMax; ++n)
						{
							Gang* M = new Gang;
							Gang* P = new Gang;

							if (spy)
							{
								M->SetChangeFreq(prob);
								P->SetChangeFreq(prob);
							}

							scoreM[4] = n;
							vector<vector<string> > statsM;
							vector<vector<string> > statsP;
							vector<string> titles;

							if (!spy)
							{
								titles.push_back("Score");
								titles.push_back("ALL_W");
								titles.push_back("ALL_X");
								titles.push_back("ALL_Y");
								titles.push_back("ALL_Z");
								titles.push_back("ALL_A");
								titles.push_back("ALL_B");
								titles.push_back("ALL_C");
							}
							else
							{
								titles.push_back("Score");
								titles.push_back("Present");
								titles.push_back("First Go");
								titles.push_back("Second Go");
								titles.push_back("Other Spy");
								titles.push_back("Both Spies");
							}
							statsM.push_back(titles);
							statsP.push_back(titles);
							try
							{
								++game;

								M->SetPrisonerStrats(strats[i], strats[j], strats[k], strats[m], strats[n]);

								if (!control)
									P = ConstructSecondGang(strats, i, j, k, m, n, scoreP);
								else
								{
									P->SetPrisonerStrats(strats[10], strats[11], strats[12], strats[13], strats[14]);
									for (int i = 0; i < 5; ++i)
										scoreP[i] = 10 + i;
								}

								int numSpies = ((float)spyProb / 100) * M->GetMaxIterations();
								vector<int> makeSpy(numSpies);

								int spies = 0;
								if (spy)
								{
									while (spies != numSpies)
									{
										int buff = rand() % M->GetMaxIterations();
										if (find(makeSpy.begin(), makeSpy.end(), buff) == makeSpy.end())
										{
											makeSpy[spies] = buff;
											++spies;
										}
									}
								}

								for (int q = 0; q < M->GetMaxIterations(); ++q)
								{
									if (spyProb != 0)
									{
										if (find(makeSpy.begin(), makeSpy.end(), q) != makeSpy.end())
										{
											M->SetSpy();
											P->SetSpy();
										}
									}
									Game(*M, *P);
								}

								scoreM[5] = M->GetScore();
								scoreP[5] = P->GetScore();
								if (dispStats || printStats)
								{
									vector<string> tableLineM;
									tableLineM.push_back(to_string(M->GetScore()));
									vector<string> tableLineP;
									tableLineP.push_back(to_string(P->GetScore()));
									if (!spy)
									{
										tableLineM.push_back(to_string(M->GetAllW()));
										tableLineM.push_back(to_string(M->GetAllX()));
										tableLineM.push_back(to_string(M->GetAllY()));
										tableLineM.push_back(to_string(M->GetAllZ()));
										tableLineM.push_back(to_string(M->GetAllA()));
										tableLineM.push_back(to_string(M->GetAllB()));
										tableLineM.push_back(to_string(M->GetAllC()));

										tableLineP.push_back(to_string(P->GetAllW()));
										tableLineP.push_back(to_string(P->GetAllX()));
										tableLineP.push_back(to_string(P->GetAllY()));
										tableLineP.push_back(to_string(P->GetAllZ()));
										tableLineP.push_back(to_string(P->GetAllA()));
										tableLineP.push_back(to_string(P->GetAllB()));
										tableLineP.push_back(to_string(P->GetAllC()));
									}
									else
									{
										tableLineM.push_back(to_string(spyProb));
										tableLineM.push_back(to_string(M->GetAllS()));
										tableLineM.push_back(to_string(M->GetAllT()));
										tableLineM.push_back(to_string(M->GetAllU()));
										tableLineM.push_back(to_string(M->GetAllV()));

										tableLineP.push_back(to_string(spyProb));
										tableLineP.push_back(to_string(P->GetAllS()));
										tableLineP.push_back(to_string(P->GetAllT()));
										tableLineP.push_back(to_string(P->GetAllU()));
										tableLineP.push_back(to_string(P->GetAllV()));
									}
									statsM.push_back(tableLineM);
									statsP.push_back(tableLineP);

									if (dispStats)
									{
										t.HLine('+', '-');
										string text = "Game " + to_string(game);
										t.AddText(text);
										text = "Gang 1: " + strats[i] + ' ' + strats[j] + ' ' + strats[k] + ' ' + strats[m] + ' ' + strats[n];
										t.AddText(text);
										if (spy)
										{
											text = "Chance of switching choice = " + to_string(prob);
											t.AddText(text);
										}
										t.DrawTable(&statsM);
										if (!control)
										{
											text = "Gang 2: " + strats[scoreP[4]] + ' ' + strats[scoreP[3]] + ' ' + strats[scoreP[2]] + ' ' + strats[scoreP[1]] + ' ' + strats[scoreP[0]];
											t.AddText(text);
											if (spy)
											{
												text = "Chance of switching choice = " + to_string(prob);
												t.AddText(text);
											}
										}
										else
											t.AddText("Control Gang");
										t.DrawTable(&statsP);
									}
									if (printStats)
									{
										string nameM = "stats";
										string titleM;
										for (int i = 0; i < 5; ++i)
										{
											titleM += strats[scoreM[i]];
											titleM += ' ';
											nameM += to_string(scoreM[i]);
										}
										nameM += ".txt";

										string nameP = "stats";
										string titleP;
										for (int i = 0; i < 5; ++i)
										{
											titleP += strats[scoreP[i]];
											titleP += ' ';
											nameP += to_string(scoreP[i]);
										}
										nameP += ".txt";

										t.PrintTable(nameM, &statsM, titleM);
										t.PrintTable(nameP, &statsP, titleP);
									}
								}
								allScores.push_back(scoreM);
								if (!control)
									allScores.push_back(scoreP);

								delete M;
								delete P;
							}
							catch (const invalid_argument& iae)
							{
								delete winners;
								delete M;
								delete P;
								throw invalid_argument(iae.what());
							}
						}
					}
				}
			}
		}
	}

	vector<int> finalScores;

	for (int i = 0; i < allScores.size(); ++i)
		finalScores.push_back(allScores[i].back());

	int winner = Min(finalScores);

	t.AddText("The best strategy combination was: ");
	string text = "";
	for (auto it = allScores[winner].begin(); it != --allScores[winner].end(); ++it)
	{
		winners->push_back(strats[*it]);
		text += strats[*it];
		text += ' ';
	}
	t.AddText(text);

	text = "With a score of " + to_string(allScores[winner].back());
	t.AddText(text);

	t.AddText("Continue...");
	char exit;
	cin >> exit;

	return winners;
}

template <typename T>
void Game(T& A, T& B)
{
	int AOutcome = A.Run();
	int BOutcome = B.Run();

	if (AOutcome >= 0)
	{
		if (BOutcome >= 0)
		{
			// if A BETRAY
			if (AOutcome == 0)
			{
				// if B BETRAY
				if (BOutcome == 0)
				{
					A.RegisterOutcome('Z');
					B.RegisterOutcome('Z');
				}
				// if B SILENCE
				else if (BOutcome == 1)
				{
					A.RegisterOutcome('Y');
					B.RegisterOutcome('X');
				}
				// if B MIXED
				else if (AOutcome < 6)
				{
					A.RegisterOutcome('A');
					B.RegisterOutcome('B');
				}
				// B discovered spy
				else
				{
					A.RegisterOutcome('U');
					// spy discovered on first attempt
					if (BOutcome == 6)
						B.RegisterOutcome('S');
					// spy discovered on second attempt
					else
						B.RegisterOutcome('T');
				}
			}
			// if A SILENCE
			else if (AOutcome == 1)
			{
				// if B BETRAY
				if (BOutcome == 0)
				{
					A.RegisterOutcome('X');
					B.RegisterOutcome('Y');
				}
				// if B SILENCE
				else if (BOutcome == 1)
				{
					A.RegisterOutcome('W');
					B.RegisterOutcome('W');
				}
				// if B MIXED
				else if (AOutcome < 6)
				{
					A.RegisterOutcome('B');
					B.RegisterOutcome('A');
				}
				// B discovered spy
				else
				{
					A.RegisterOutcome('U');
					// spy discovered on first attempt
					if (BOutcome == 6)
						B.RegisterOutcome('S');
					// spy discovered on second attempt
					else
						B.RegisterOutcome('T');
				}
			}
			// if A MIXED
			else if (AOutcome < 6)
			{
				// if B had more BETRAYs
				if ((BOutcome == 0) || (BOutcome > AOutcome))
				{
					A.RegisterOutcome('B');
					B.RegisterOutcome('A');
				}
				// if B and A had the same number of BETRAYs
				else if (BOutcome == AOutcome)
				{
					A.RegisterOutcome('C');
					B.RegisterOutcome('C');
				}
				// if B had fewer BETRAYs
				else if (AOutcome < 6)
				{
					A.RegisterOutcome('A');
					B.RegisterOutcome('B');
				}
				// B discovered spy
				else
				{
					A.RegisterOutcome('U');
					// spy discovered on first attempt
					if (BOutcome == 6)
						B.RegisterOutcome('S');
					// spy discovered on second attempt
					else
						B.RegisterOutcome('T');
				}

			}
			// A discovered spy
			else
			{
				// B didn't discover spy
				if (BOutcome < 6)
				{
					B.RegisterOutcome('U');
					// spy discovered on first attempt
					if (AOutcome == 6)
						A.RegisterOutcome('S');
					// spy discovered on second attempt
					else
						A.RegisterOutcome('T');
				}
				// B also discovered spy
				else
				{
					A.RegisterOutcome('V');
					B.RegisterOutcome('V');
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

Gang * ConstructSecondGang(vector<string>& strats, const int i, const int j, const int k, const int m, const int n, vector<int>& score) throw (invalid_argument)
{
	int index[5];

	int ind = 0;
	for (int u = 9; u > n; --u)
	{
		index[ind] = u;
		score[ind] = u;
		++ind;
	}
	for (int u = n - 1; u > m; --u)
	{
		index[ind] = u;
		score[ind] = u;
		++ind;
	}
	for (int u = m - 1; u > k; --u)
	{
		index[ind] = u;
		score[ind] = u;
		++ind;
	}
	for (int u = k - 1; u > j; --u)
	{
		index[ind] = u;
		score[ind] = u;
		++ind;
	}
	for (int u = j - 1; u > i; --u)
	{
		index[ind] = u;
		score[ind] = u;
		++ind;
	}
	for (int u = i - 1; u >= 0; --u)
	{
		index[ind] = u;
		score[ind] = u;
		++ind;
	}

	try
	{
		Gang* Second = new Gang(strats[index[0]], strats[index[1]], strats[index[2]], strats[index[3]], strats[index[4]]);

		return Second;
	}
	catch (const invalid_argument& iae)
	{
		throw invalid_argument(iae.what());
	}
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
