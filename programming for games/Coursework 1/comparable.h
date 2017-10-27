// Will Hinds, Computer Games Engineering MSc - comparable.h //
// --------------------------------------------------------- //
// Contains class definitions for comparable objects         //
// Student ID: 170740805, Date: 27/10/17 10:00               //

#pragma once

#include <iostream>

using namespace std;

// pure abstract Comparable class to allow for comparison of objects stored in BST
class Comparable
{
public:
	virtual int CompareTo(const Comparable& rhs) = 0;
};


// Integer Comparable used tot store line numbers
class Integer : public Comparable
{
public:
	Integer() : value(0) { }
	Integer(int val) : value(val) { }
	~Integer() { }
	inline int GetValue() { return value; }
	inline void SetValue(int val) { value = val; }
	virtual int CompareTo(const Comparable& rhs) override;
	Integer& operator=(const Comparable& rhs);
private:
	int value;
};

int Integer::CompareTo(const Comparable& rhs)
{
	const Integer *i = dynamic_cast<const Integer*>(&rhs);

	if (this->value == i->value)
		return 0;
	else if (this->value > i->value)
		return 1;
	else
		return -1;
}

Integer& Integer::operator=(const Comparable& rhs)
{
	const Integer *i = dynamic_cast<const Integer*>(&rhs);

	if (this == &rhs)
		return *this;

	value = i->value;

	return *this;
}
