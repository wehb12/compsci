// Will Hinds, Computer Games Engineering MSc - BST.h //
// -------------------------------------------------- //
// Contains class definitions for BST   	          //

#pragma once

#include <iostream>
#include "comparable.h"

using namespace std;

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
	BST(T value);
	BST();
	~BST();
	void Insert(struct node<T>* tree, const T& leaf);
	void Print(struct node<T>* tree);
	void Terminate(struct node<T>* tree);
	bool Find(struct node<T>* tree, const T& leaf);
	inline node<T>* GetRoot() { return root; }
	inline void SetRoot(const T& leaf) { root->value = leaf; }
private:
	int populated;
	node<T>* root;
};

template <typename T>
BST<T>::BST(T value)
{
	root = new node<T>;
	root->value = value;
	root->right = NULL;
	root->left = NULL;
	populated = 1;
}

template <typename T>
BST<T>::BST()
{
	root = new node<T>;
	root->right = NULL;
	root->left = NULL;
	populated = 1;
}

template <typename T>
BST<T>::~BST()
{
	if (populated != 0)
		Terminate(root);
}

template <typename T>
void BST<T>::Insert(struct node<T>* tree, const T& leaf)
{
	populated = 1;
	int comparison = tree->value.CompareTo(leaf);

	if (comparison == -1)
	{
		if (tree->right != NULL)
			Insert(tree->right, leaf);
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
			Insert(tree->left, leaf);
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
void BST<T>::Print(struct node<T>* tree)
{
	if (tree->left != NULL)
		Print(tree->left);
	cout << tree->value.GetValue() << endl;
	if (tree->right != NULL)
		Print(tree->right);
}

template <typename T>
void BST<T>::Terminate(struct node<T>* tree)
{
	if (tree->left != NULL)
		Terminate(tree->left);
	if (tree->right != NULL)
		Terminate(tree->right);
	delete tree;

	populated = 0;
}

template <typename T>
bool BST<T>::Find(struct node<T>* tree, const T& leaf)
{
	int comparison = tree->value.CompareTo(leaf);

	if (comparison == 0)
		return true;
	else if (comparison == 1)
	{
		if (tree->left != NULL)
			return Find(tree->left, leaf);
		else return false;
	}
	else
	{
		if (tree->right != NULL)
			return Find(tree->right, leaf);
		else return false;
	}
}

//int main()
//{
//	BST<Integer> tree(5);
//
//	Integer leaf1(8);
//	Integer leaf2(10);
//	Integer leaf3(4);
//	Integer leaf4(6);
//	Integer leaf5(9);
//	Integer leaf6(27);
//	Integer leaf7(3);
//	Integer leaf8(7);
//
//	tree.insert_integer(tree.get_root(), leaf1);
//	tree.insert_integer(tree.get_root(), leaf2);
//	tree.insert_integer(tree.get_root(), leaf3);
//	tree.insert_integer(tree.get_root(), leaf4);
//	tree.insert_integer(tree.get_root(), leaf5);
//	tree.insert_integer(tree.get_root(), leaf6);
//	tree.insert_integer(tree.get_root(), leaf7);
//	tree.insert_integer(tree.get_root(), leaf8);
//
//	cout << "Enter a value to find: ";
//	int val;
//	cin >> val;
//
//	Integer find(val);
//
//	cout << endl << "Value present?: ";
//
//	if (tree.find(tree.get_root(), find))
//		cout << "TRUE";
//	else cout << "FALSE";
//	cout << endl;
//
//	tree.print_tree(tree.get_root());
//	tree.terminate_tree(tree.get_root());
//	cout << "DELETED" << endl;
//	return 0;
//}