// stack.cpp created by Will Hinds

#include <iostream>

using namespace std;

template <typename T>
class Stack
{
protected:
    int length;
    T* elem;
public:
    inline Stack() : length(0) { elem = new T; }
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
    --length;
    
    delete (elem + length);
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
        *elem = data;
        return;
    }
        
    T* buff = elem;
    
    delete[] elem;
    
    elem = new T[length];
    
    for (int i = length - 2; i >= 0; --i)
        *(elem + i) = *(buff + i);
    
    *(elem + length - 1) = *(buff + length - 1);
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
}
