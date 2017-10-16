// tree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"

using namespace std;

class Comparable
{
public:
	virtual int compare_to(const Comparable& rhs) = 0;
};

class Data : public Comparable
{
public:
	Data() : value(0) { }
	Data(int val) : value(val) { }
	~Data() { }
	int get_value() { return value; }
	void set_value(int val) { value = val; }
	virtual int compare_to(const Comparable& rhs) override;
	Data& operator=(const Comparable& rhs);
private:
	int value;
};

int Data::compare_to(const Comparable& rhs)
{
	const Data *d = dynamic_cast<const Data*>(&rhs);	//dynamic_cast doesn't have the ability to remove a const qualifier

	if (this->value == d->value)
		return 0;
	else if (this->value > d->value)
		return 1;
	else if (this->value < d->value)
		return -1;
}

Data& Data::operator=(const Comparable& rhs)
{
	const Data *d = dynamic_cast<const Data*>(&rhs);

	if (this == &rhs)
		return *this;

	value = d->value;
}

template <typename T>
struct node
{
	T value;
	struct node* left;
	struct node* right;
};

template <typename T>
class BST
{
public:
	static const int defValue = 0;
	BST(int value = defValue);
	~BST();
	void insert_integer(struct node<T>* tree, const T& leaf);
	void print_tree(struct node<T>* tree);
	void terminate_tree(struct node<T>* tree);
	bool find(struct node<T>* tree, const T& leaf);
	node<T>* get_root();
private:
	int populated;
	node<T>* root;
};

template <typename T>
BST<T>::BST(int value)
{
	root = new node<T>;
	root->value.set_value(value);
	root->right = NULL;
	root->left = NULL;
	populated = 1;
}

template <typename T>
BST<T>::~BST()
{
	if (populated != 0)
		terminate_tree(root);
}

template <typename T>
void BST<T>::insert_integer(struct node<T>* tree, const T& leaf)
{
	populated = 1;
	int comparison = tree->value.compare_to(leaf);

	if (comparison == -1)
	{
		if (tree->right != NULL)
			insert_integer(tree->right, leaf);
		else
		{
			tree->right = new node<T>;
			tree->right->value = leaf;
			tree->right->left = NULL;
			tree->right->right = NULL;
		}
	}
	else if ((comparison == 1) || (comparison == 0))
	{
		if (tree->left != NULL)
			insert_integer(tree->left, leaf);
		else
		{
			tree->left = new node<T>;
			tree->left->value = leaf;
			tree->left->left = NULL;
			tree->left->right = NULL;
		}
	}
}

template <typename T>
void BST<T>::print_tree(struct node<T>* tree)
{
	if (tree->left != NULL)
		print_tree(tree->left);
	cout << tree->value.get_value() << endl;
	if (tree->right != NULL)
		print_tree(tree->right);
}

template <typename T>
void BST<T>::terminate_tree(struct node<T>* tree)
{
	if (tree->left != NULL)
		terminate_tree(tree->left);
	if (tree->right != NULL)
		terminate_tree(tree->right);
	delete tree;

	populated = 0;
}

template <typename T>
bool BST<T>::find(struct node<T>* tree, const T& leaf)
{
	int comparison = tree->value.compare_to(leaf);

	if (comparison == 0)
		return true;
	else if (comparison == 1)
	{
		if (tree->left != NULL)
			return find(tree->left, leaf);
		else return false;
	}
	else
	{
		if (tree->right != NULL)
			return find(tree->right, leaf);
		else return false;
	}
}

template <typename T>
node<T>* BST<T>::get_root()
{
	return root;
}

int main()
{
	BST<Data> tree(5);

	Data leaf1(8);
	Data leaf2(10);
	Data leaf3(4);
	Data leaf4(6);
	Data leaf5(9);
	Data leaf6(27);
	Data leaf7(3);
	Data leaf8(7);

	tree.insert_integer(tree.get_root(), leaf1);
	tree.insert_integer(tree.get_root(), leaf2);
	tree.insert_integer(tree.get_root(), leaf3);
	tree.insert_integer(tree.get_root(), leaf4);
	tree.insert_integer(tree.get_root(), leaf5);
	tree.insert_integer(tree.get_root(), leaf6);
	tree.insert_integer(tree.get_root(), leaf7);
	tree.insert_integer(tree.get_root(), leaf8);

	cout << "Enter a value to find: ";
	int val;
	cin >> val;

	Data find(val);

	cout << endl << "Value present?: ";

	if (tree.find(tree.get_root(), find))
		cout << "TRUE";
	else cout << "FALSE";
	cout << endl;

	tree.print_tree(tree.get_root());
	tree.terminate_tree(tree.get_root());
	cout << "DELETED" << endl;
	return 0;
}
