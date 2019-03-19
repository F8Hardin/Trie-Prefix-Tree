#include "pch.h"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

stringstream ss;

class Trie {
private:
	string words;	//the string that the trie node contains
	bool isTerminal;	//
	Trie *kids[26];		//the array of nodes, 26 for each possible letter of the alphabet
	Trie *newNode(string);		//creates a new node based off the given string from the insert function
	int findDist(Trie*, Trie*);		//travese the list down to current keeping track of how many levels deep for how the length of x needed in newBranch

public:
	Trie();		//sets the root = nullptr to get the trie ready
	void insert(string);	//inserts the given string into the parse tree
	void insertWork(string, Trie*);		//does all the work for the insert function, finding the appropriate way to disperse the word given among the tree
	void show(Trie*);	//called by the print function, cycles through the trie to show everything off
	void print(); //calls show funtion
	Trie *root; //the root of the trie
	void newBranch(string, Trie*, int); //places new parts of the string into the trie
};

void Trie::newBranch(string x, Trie* temp, int index) {
	string z = x.substr(0, findDist(temp, root));
	cout << z << endl;
	temp->kids[index] = newNode(z);
	if (temp->kids[index]->words.length() != x.length()) {
		return newBranch(x, temp->kids[index], 0);
	}
}

Trie::Trie() {
	root = nullptr;
}

int Trie::findDist(Trie* current, Trie* temp) {
	if (current == temp)
		return 1;
	for (int i = 0; i < 26; i++) {
		if (temp->kids[i]) {
			if (current->words.find(temp->kids[i]->words) != string::npos)
				return 1 + findDist(current, temp->kids[i]);
		}
	}
}


void Trie::insert(string x) {
	insertWork(x, root);
}

void Trie::insertWork(string x, Trie *temp) {
	if (!root) {
		string z;
		ss << x[0];
		ss >> z;
		root = newNode("");
		newBranch(x, root, 0);
	}
	else {
		bool noMore = false;
		int index = -1; //index where array is now empty
		for (int i = 0; i < 26; i++) {
			if (temp->kids[i]) {
				if (x.find(temp->kids[i]->words) != string::npos) {
					return insertWork(x, temp->kids[i]);
				}
			}
			else if (noMore == false) {
				index = i;
				noMore = true;
			}
		}
		newBranch(x, temp, index);
	}
}

Trie* Trie::newNode(string x) {
	Trie *temp = new Trie;
	temp->words = x;
	temp->isTerminal = false;
	return temp;
}

void Trie::show(Trie* temp) {
	if (!temp)
		return;
	else {
		cout << "===================" << endl;
		cout << temp->words + ": ";
		for (int i = 0; i < 26; i++) {
			if (temp->kids[i]) {
				cout << temp->kids[i]->words + ",";
			}
		}
		cout << endl;
		//for (int i = 0; i < 26; i++) {
		//	if (temp->kids[i])
		//		show(temp->kids[i]);
		//}
	}
}

void Trie::print() {
	show(root);
}

int main()
{
	Trie tree;
	tree.insert("have");
	tree.insert("seven");
	tree.insert("cupcake");
	tree.insert("chicken");
	tree.insert("sheet");
	tree.insert("caterpillar");
	tree.insert("sleep");
	tree.insert("rainbow");
	tree.print();
	string stop;
	cin >> stop;
}