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

//struct node* find_parent(struct node* root, int* par_dir, int value);
void print_tree(struct node* tree);

//struct node* inorder_successor(struct node*);
//void terminate_node(struct node* tree, struct node* root);
//void terminate_tree(struct node* tree);

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

//void terminate_node(struct node* tree, struct node* root)
//{
//	struct node* parent;
//	struct node* temp = tree;
//	int par_dir;	//parent direction = 0 for not a parent = 1 for right handed parent = 2 for left handed parent = 3 for root
//
//	while (1)
//	{
//		parent = find_parent(root, &par_dir, tree->value);
//
//		if (par_dir == 3)
//			cout << "Don't delete the root" << endl;
//		else if ((tree->left == NULL) && (tree->right == NULL))	//node is a leaf, so just delete
//		{
//			delete tree;
//			return;
//		}
//		else if ((tree->left == NULL) || (tree->right == NULL))	//node has only one branch, so shift them up
//		{
//			switch (par_dir)
//			{
//			case(1):
//				if (tree->left == NULL)
//					parent->left = tree->right;
//				else parent->left = tree->left;
//				break;
//			case(2):
//				if (tree->left == NULL)
//					parent->right = tree->right;
//				else parent->right = tree->left;
//				break;
//			}
//			delete tree;
//			return;
//		}
//		else
//		{
//			tree->value = temp->value;
//			temp = inorder_successor;		//could delete in order successor by pointer?
//											//use the while loop here?
//											//could definitely restructure to work, I think
//
//		}
//	}
//}

//struct node* inorder_successor(struct node* tree)
//{
//	tree = tree->right;
//	while (1)
//	{
//		if (tree->left != NULL)
//			tree = tree->left;
//		else return tree;
//	}
//}

//struct node* search(struct node* root, int value)
//{
//	int found = 0;
//
//	while (!found)
//	{
//
//	}
//}

//void terminate_tree(struct node* tree)
//{
//	if (leaf->left != NULL)
//		terminate_leaf(leaf->left);
//	if (leaf->right != NULL)
//		terminate_leaf(leaf->right);
//	delete leaf;
//}
