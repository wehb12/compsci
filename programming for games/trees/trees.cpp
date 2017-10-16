// comparables.cpp : Defines the entry point for the console application.
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

struct node
{
	Data value;
	struct node* left;
	struct node* right;
};

class BST
{
public:
	static const int defValue = 0;
	BST(int value = defValue);
	~BST();
	void insert_integer(struct node* tree, const Comparable& leaf);
	void print_tree(struct node* tree);
	void terminate_tree(struct node* tree);
	bool find(struct node* tree, const Comparable& leaf);
	node* get_root();
private:
	int populated;
	node* root;
};

BST::BST(int value)
{
	root = new node;
	root->value.set_value(value);
	root->right = NULL;
	root->left = NULL;
	populated = 1;
}

BST::~BST()
{
	if (populated != 0)
		terminate_tree(root);
}

void BST::insert_integer(struct node* tree, const Comparable& leaf)
{
	populated = 1;
	int comparison = tree->value.compare_to(leaf);

	if (comparison == -1)
	{
		if (tree->right != NULL)
			insert_integer(tree->right, leaf);
		else
		{
			tree->right = new node;
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
			tree->left = new node;
			tree->left->value = leaf;
			tree->left->left = NULL;
			tree->left->right = NULL;
		}
	}
}

void BST::print_tree(struct node* tree)
{
	if (tree->left != NULL)
		print_tree(tree->left);
	cout << tree->value.get_value() << endl;
	if (tree->right != NULL)
		print_tree(tree->right);
}

void BST::terminate_tree(struct node* tree)
{
	if (tree->left != NULL)
		terminate_tree(tree->left);
	if (tree->right != NULL)
		terminate_tree(tree->right);
	delete tree;

	populated = 0;
}

bool BST::find(struct node* tree, const Comparable& leaf)
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

node* BST::get_root()
{
	return root;
}

int main()
{
	BST tree(5);

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
