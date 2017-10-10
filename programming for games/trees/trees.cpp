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

void insert_integer(struct node* tree, int value);
void print_tree(struct node* tree);
void terminate_tree(struct node* tree);

int main()
{
	struct node *root = new node;			//make root on heap so it can be reallocated using 'delete'
	*root = { 0 };

	root->value = 5;
	insert_integer(root, 8);
	insert_integer(root, 10);
	insert_integer(root, 4);
	insert_integer(root, 6);
	insert_integer(root, 9);
	insert_integer(root, 27);
	insert_integer(root, 3);
	insert_integer(root, 7);

	print_tree(root);
	terminate_tree(root);
	cout << "DELETED" << endl;
    return 0;
}

void insert_integer(struct node* tree, int value)
{
	if( tree->value < value )
	{
		if( tree->right != NULL )
			insert_integer(tree->right, value);
		else
		{
			tree->right = new node;
			tree->right->value = value;
			tree->right->left = NULL;
			tree->right->right = NULL;
		}
	}
	else if(tree->value >= value)
	{
		if( tree->left != NULL)
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

void print_tree(struct node* tree)
{
	if (tree->left != NULL)
		print_tree(tree->left);
	cout << tree->value << endl;
	if (tree->right != NULL)
		print_tree(tree->right);
}

void terminate_tree(struct node* tree)
{
	if (tree->left != NULL)
		terminate_tree(tree->left);
	if (tree->right != NULL)
		terminate_tree(tree->right);
	delete tree;
}