// trees.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"

using namespace std;

struct node
{
	int value;
	struct node* left;
	struct node* right;
};

class BST
{
public:
	static const int defValue = 0;
	BST(int value = defValue);
	~BST();
	void insert_integer(struct node* tree, int value);
	void print_tree(struct node* tree);
	void terminate_tree(struct node* tree);
	bool find(struct node* tree, int value);
	node* get_root();
private:
	int populated;
	node* root;
};

BST::BST(int value)
{
	root = new node;
	root->value = value;
	root->right = NULL;
	root->left = NULL;
	populated = 1;
}

BST::~BST()
{
	if (populated != 0)
		terminate_tree(root);
}

void BST::insert_integer(struct node* tree, int value)
{
	populated = 1;

	if (tree->value < value)
	{
		if (tree->right != NULL)
			insert_integer(tree->right, value);
		else
		{
			tree->right = new node;
			tree->right->value = value;
			tree->right->left = NULL;
			tree->right->right = NULL;
		}
	}
	else if (tree->value >= value)
	{
		if (tree->left != NULL)
			insert_integer(tree->left, value);
		else
		{
			tree->left = new node;
			tree->left->value = value;
			tree->left->left = NULL;
			tree->left->right = NULL;
		}
	}
}

void BST::print_tree(struct node* tree)
{
	if (tree->left != NULL)
		print_tree(tree->left);
	cout << tree->value << endl;
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

bool BST::find(struct node* tree, const int value)
{
	if (tree->value == value)
		return true;
	else if (tree->value > value)
	{
		if (tree->left != NULL)
			return find(tree->left, value);
		else return false;
	}
	else
	{
		if (tree->right != NULL)
			return find(tree->right, value);
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

	tree.insert_integer(tree.get_root(), 8);
	tree.insert_integer(tree.get_root(), 10);
	tree.insert_integer(tree.get_root(), 4);
	tree.insert_integer(tree.get_root(), 6);
	tree.insert_integer(tree.get_root(), 9);
	tree.insert_integer(tree.get_root(), 27);
	tree.insert_integer(tree.get_root(), 3);
	tree.insert_integer(tree.get_root(), 7);

	cout << "Enter a value to find: ";
	int val;
	cin >> val;

	cout << endl << "Value present?: ";
	
	if (tree.find(tree.get_root(), val))
		cout << "TRUE";
	else cout << "FALSE";
	cout << endl;

	tree.print_tree(tree.get_root());
	tree.terminate_tree(tree.get_root());
	cout << "DELETED" << endl;
    return 0;
}
