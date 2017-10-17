// enemy_player.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "string"

using namespace std;

class Enemy {
public:
	Enemy(int hps, int scr);
	~Enemy();
	int get_hit_points() const;
	friend ostream& operator<<(ostream& ostr, const Enemy& enem);
	int get_score() const;
	void set_hit_points(int new_hit_points);
	void set_score(int new_score);
protected:
	int hit_points;
	int* score;
};

Enemy::Enemy(int hps, int scr) :
	hit_points(hps) {
	score = new int(0);
	*score = scr;
}

Enemy::~Enemy() {
	delete score;
}

int Enemy::get_hit_points() const {
	return hit_points;
}

int Enemy::get_score() const {
	return *score;
}

void Enemy::set_hit_points(const int new_hit_points) {
	hit_points = new_hit_points;
}

void Enemy::set_score(const int new_score) {
	*score = new_score;
}

ostream& operator<<(ostream& ostr, const Enemy& enem)
{
	ostr << "Hit Points: " << enem.hit_points << endl;
	ostr << "Score: " << *enem.score << endl;

	return ostr;
}

class Player : public Enemy
{
public:
	Player(int hps, int scr);
	~Player();
	friend ostream& operator<<(ostream& ostr, const Player& plyr);
	void set_name(string nam);
	string get_name() const;
private:
	string* name;
};

Player::Player(int hps, int scr) :
	Enemy(hps, scr)
{
	name = new string("");
}

Player::~Player()
{
	delete name;
	name = NULL;
}

void Player::set_name(string nam)
{
	*name = nam;
}

string Player::get_name() const
{
	return *name;
}

//if plyr is passed by value, not reference, another Player object is created, but with same memory locations
//so after this function finishes, plyr goes out of scope and calls the destructor
//this deletes the memory locations associated with the initial passed in p1 object
//so when p1 tries to destructl, the memory is unassigned so it throws an exception
ostream& operator<<(ostream& ostr, const Player& plyr)
{
	ostr << "Name: " << plyr.get_name() << endl;
	ostr << "Hit Points: " << plyr.get_hit_points() << endl;
	ostr << "Score: " << plyr.get_score() << endl;

	return ostr;
}

int main(void)
{
	Player p1(5, 0);
	cout << "What's you name?" << endl;
	string buff;
	getline(cin, buff);
	p1.set_name(buff);

	cout << p1;

	return 0;
}
