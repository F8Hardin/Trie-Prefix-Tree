//Fate Hardin
//CSCI 490: Trie Prefix Tree Implementation
#include "pch.h"
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

stringstream ss;

class Trie {
private:
	string words;	//the string that the trie node contains
	bool isTerminal;	//set to true if the node's word is a terminal
	Trie *kids[26];		//the array of nodes, 26 for each possible letter of the alphabet
	Trie *newNode(string);		//creates a new node based off the given string from the insert function
	int findDist(Trie*, Trie*);		//travese the list down to current keeping track of how many levels deep for how the length of x needed in newBranch
	void insertWork(string, Trie*);		//does all the work for the insert function, finding the appropriate way to disperse the word given among the tree
	void show(Trie*);	//called by the print function, cycles through the trie to show everything off
	void newBranch(string, Trie*, int, int);	//places new parts of the string into the trie
	Trie *root;		//the root of the trie

public:
	Trie();		//sets the root = nullptr to get the trie ready
	void insert(string);	//inserts the given string into the parse tree
	void print();	//calls show funtion
};

Trie::Trie() {
	root = nullptr;
}

void Trie::newBranch(string x, Trie* temp, int index, int dist) {
	string z = x.substr(0, dist);	//adding one char at a time onto a new branch
	if (temp->words != z) {		//if the word is already in the tree
		temp->kids[index] = newNode(z);
		if (temp->kids[index]->words == x)
			temp->kids[index]->isTerminal = true;
		if (temp->kids[index]->words.length() != x.length()) {
			return newBranch(x, temp->kids[index], 0, dist + 1);	//add 1 to the string size for the next node
		}	
	}
}

int Trie::findDist(Trie* current, Trie* temp) {
	if (current == temp)
		return 1;
	for (int i = 0; i < 26; i++) {
		if (temp->kids[i]) {
			if (current->words.find(temp->kids[i]->words) != string::npos && current->words[0] == temp->kids[i]->words[0])
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
		newBranch(x, root, 0, 1);
	}
	else {
		bool noMore = false; //stops the for loop when index is updated
		int index = -1; //index where array is now empty
		for (int i = 0; i < 26; i++) {
			if (temp->kids[i]) {
				if (x.find(temp->kids[i]->words) != string::npos && temp->kids[i]->words[0] == x[0]) { //update to this node if it is a substring of x and starts with the same char
					return insertWork(x, temp->kids[i]);
				}
			}
			else if (noMore == false) { //if the alphabet array is empty
				index = i;
				noMore = true;
			}
		}
		newBranch(x, temp, index, findDist(temp, root));
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
		if (temp->isTerminal == true)
			cout << "TERMINAL: ";
		cout << temp->words + ": ";
		for (int i = 0; i < 26; i++) {
			if (temp->kids[i]) {
				cout << temp->kids[i]->words + ",";
			}
		}
		cout << endl;
		for (int i = 0; i < 26; i++) {
			if (temp->kids[i])
				show(temp->kids[i]);
		}
	}
}

void Trie::print() {
	cout << "Parent Node: Children" << endl;
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
	tree.insert("sheets");
	tree.insert("caterpillar");
	tree.insert("sleep");
	tree.insert("rainbow");
	tree.insert("1010");
	tree.insert("0000");
	tree.insert("1010");
	tree.insert("0101");
	tree.insert("010101");
	tree.print();
	string stop;
	cin >> stop;
}