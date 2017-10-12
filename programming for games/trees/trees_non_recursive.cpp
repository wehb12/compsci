// trees_non_recursive.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include <stack>

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
	terminate_tree(root);
	cout << "DELETED" << endl;
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

void print_tree(struct node* root)
{
	struct node temp = { 0 };
	stack<node> treeStack;

	if (root == NULL)
		return;

	treeStack.push(*root);

	while (!treeStack.empty())
	{
		if (treeStack.top().right != NULL)
		{
			temp = treeStack.top();
			treeStack.pop();
			treeStack.push(*temp.right);
			treeStack.push(temp);
			treeStack.top().right = NULL;
		}
		else if (treeStack.top().left != NULL)
		{
			temp = *treeStack.top().left;
			treeStack.top().left = NULL;
			treeStack.push(temp);
		}
		else
		{
			cout << treeStack.top().value << endl;
			treeStack.pop();
		}
	}
}

void terminate_tree(struct node* root)
{
	struct node* temp = { 0 };

	stack<node*> tree;

	if (root == NULL)
		return;

	tree.push(root);

	while (!tree.empty())
	{
		if (tree.top()->right != NULL)
		{
			temp = tree.top();
			tree.pop();
			tree.push(temp->right);
			tree.push(temp);
			tree.top()->right = NULL;
		}
		else if (tree.top()->left != NULL)
		{
			temp = tree.top()->left;
			tree.top()->left = NULL;
			tree.push(temp);
		}
		else
		{
			delete tree.top();
			tree.pop();
		}
	}
}
