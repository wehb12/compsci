// trees_non_recursive.cpp : Defines the entry point for the console application.
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
//void terminate_tree(struct node* tree);

struct node* find_parent(struct node* root, int* par_dir, int value);

int main()
{
	struct node* root = new node;

	root->value = 5;
	root->left = NULL;
	root->right = NULL;

	insert_integer(root, 8);
	insert_integer(root, 10);
	insert_integer(root, 4);
	insert_integer(root, 6);
	insert_integer(root, 9);
	insert_integer(root, 27);
	insert_integer(root, 3);
	insert_integer(root, 7);

	print_tree(root);
	//terminate_tree(root);
	//cout << "DELETED" << endl;
	return 0;
}

void insert_integer(struct node* tree, int value)
{
	node* leaf = tree;
	int exitFlag = 0;
	while (!exitFlag)
	{
		if (leaf->value < value)
		{
			if (leaf->right != NULL)
				leaf = leaf->right;
			else
			{
				leaf->right = new node;
				leaf->right->value = value;
				leaf->right->left = NULL;
				leaf->right->right = NULL;
				exitFlag = 1;
			}
		}
		else if (leaf->value >= value)
		{
			if (leaf->left != NULL)
				leaf = leaf->left;
			else
			{
				leaf->left = new node;
				leaf->left->value = value;
				leaf->left->left = NULL;
				leaf->left->right = NULL;
				exitFlag = 1;
			}
		}
	}
}

void print_tree(struct node* tree)
{
	node* root = tree;
	node* leaf = tree;

	int par_dir = 0;	//parent direction = 0 for not a parent = 1 for right handed parent = 2 for left handed parent = 3 for root

	if (leaf == NULL)
		return;

	while (par_dir != 3)
	{
		switch(par_dir)
		{
			case 0:
				if (leaf->left != NULL)
				{
					leaf = leaf->left;
					par_dir = 0;
				}
				else
				{
					cout << leaf->value << endl;
					if (leaf->right != NULL)
						leaf = leaf->right;
					else
						leaf = find_parent(root, &par_dir, leaf->value);
				}
				break;
			case 1:
				cout << leaf->value << endl;
				if (leaf->right != NULL)
				{
					leaf = leaf->right;
					par_dir = 0;
				}
				else
					leaf = find_parent(root, &par_dir, leaf->value);
				break;
			case 2:
				leaf = find_parent(root, &par_dir, leaf->value);
				break;
		}
	}
}

//this function finds the parent of a node with value = value,
//sets the par_dir (parent direction) flag and then returns the parent node
struct node* find_parent(struct node* root, int* par_dir, int value)
{
	struct node* tree = root;

	int ans = 3;

	while (1)
	{
		if (tree->value >= value)
		{
			if (tree->value == value)
			{
				*par_dir = 3;
				return tree;
			}
			else if (tree->left->value != value)
				tree = tree->left;
			else if (tree->left->value == value)
			{
				*par_dir =  1;
				return tree;
			}
		}
		else if (tree->value < value)
		{
			if (tree->right->value != value)
				tree = tree->right;
			else if (tree->right->value == value)
			{
				*par_dir =  2;
				return tree;
			}
		}
	}
}

//void terminate_tree(struct node* tree)
//{
//	if (leaf->left != NULL)
//		terminate_leaf(leaf->left);
//	if (leaf->right != NULL)
//		terminate_leaf(leaf->right);
//	delete leaf;
//}
