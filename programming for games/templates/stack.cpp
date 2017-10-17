// stack.cpp created by Will Hinds

#include "stdafx.h"
#include <iostream>

using namespace std;

template <typename T>
class Stack
{
protected:
	int length;
	T* elem;
public:
	inline Stack() : length(0) { }
	~Stack();
	virtual void push(T data);// = 0;
	void pop();
	inline T top() { return *(elem + length - 1); }
	inline int size() { return size; }
	bool empty();
};

template <typename T>
Stack<T>::~Stack()
{
	if (length != 0)
		delete[] elem;

	elem = NULL;
}

template <typename T>
void Stack<T>::pop()
{
	*(elem + length - 1) = NULL;
	--length;
}

template <typename T>
bool Stack<T>::empty()
{
	if (length == 0)
		return true;
	else return false;
}

//template <typename T>
//class StackContig : public Stack<T>
//{
//public:
//    inline StackContig() : Stack<T>() { }
//    virtual void push(T data) override;
//};

template <typename T>
void Stack<T>::push(T data)
{
	++length;

	if (length == 1)
	{
		elem = new T;
		*elem = data;
		return;
	}

	//T* buff = new T[length - 1];
	T* buff = elem;

	//for (int i = 0; i < length - 1; ++i)
	//	*(buff + i) = *(elem + i);

	//delete[] elem;
	elem = NULL;

	elem = new T[length];

	for (int i = length - 2; i >= 0; --i)
		*(elem + i) = *(buff + i);

	*(elem + length - 1) = data;

	delete[] buff;
	buff = NULL;
}

int main()
{
	Stack<int> stack;

	stack.push(2);
	stack.push(1);
	stack.push(3);
	stack.push(1);

	while (!stack.empty())
	{
		cout << stack.top() << endl;
		stack.pop();
	}

	return 0;
}
