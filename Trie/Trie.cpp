//Fate Hardin
//CSCI 490: Trie Prefix Tree Implementation
#include "pch.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <list>

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
	bool findHelper(string, Trie*);		//helps search see if a node is in the trie
	void findRHelper(string, list<string>, Trie*);	//helps findReplace determine what the new sentence will be

public:
	Trie();		//sets the root = nullptr to get the trie ready
	void insert(string);	//inserts the given string into the parse tree
	void findReplace(string, list<string>);
	void print();	//calls show funtion
	void search(string);	//checks to see if the given string is in the tree
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

void Trie::search(string s) {
	if (findHelper(s, root))
		cout << "Found" << endl;
	else
		cout << "Not found" << endl;
}

bool Trie::findHelper(string s, Trie* temp) {
	if (s != temp->words) {
		for (int i = 0; i < 26; i++) {
			if (temp->kids[i]) {
				if (s.find(temp->kids[i]->words) != string::npos && s[0] == temp->kids[i]->words[0])
					return findHelper(s, temp->kids[i]);
			}
			else
				return false;
		}
	}
	else if (s != "")
		return true;
	else
		return false;
}

void Trie::print() {
	cout << "Parent Node: Children" << endl;
	show(root);
}

void Trie::findReplace(string s, list<string> dict) {
	findRHelper(s, dict, root);
}

void Trie::findRHelper(string s, list<string> dict, Trie* temp) {
	if (find(dict.begin(), dict.end(), temp->words) != dict.end())
		cout << temp->words << " ";
	else if (!temp->kids[0])
		cout << temp->words << " ";
	else
		for (int i = 0; i < 26; i++) {
			if (temp->kids[i]) {
				if (s.find(temp->kids[i]->words) != string::npos && s[0] == temp->kids[i]->words[0])
					return findRHelper(s, dict, temp->kids[i]);
			}
		}
}

void printSent(string sent, list<string>& dict) { //replaces the words in the sentence with they're substrings found in the dict by checking the tree
	Trie tree;

	istringstream iss(sent); //split everything over spaces and store it into s
	vector<string> s;
	string temp;
	iss >> temp;
	while (iss) {
		s.push_back(temp);
		iss >> temp;
	}

	for (auto const& value : s) {
		tree.insert(value);
	}

	for (auto const& value : s) {
		tree.findReplace(value, dict);
	}

	cout << endl;
	tree.print();
}

int main()
{
	Trie tree;
	//Testing inserting into the tree
	cout << "================" << endl;
	cout << "Basic Word Tree:" << endl;
	cout << "================" << endl;
	tree.insert("have");
	tree.insert("seven");
	tree.insert("cupcake");
	tree.insert("chicken");
	tree.insert("sheet");
	tree.insert("sheets");
	tree.insert("caterpillar");
	tree.insert("sleep");
	tree.insert("rainbow");
	tree.print();
	cout << "================" << endl;
	cout << "Testing Search:" << endl;
	cout << "================" << endl;
	tree.search("cows");
	tree.search("sleep");
	tree.search("rain");
	tree.search("");

	Trie tree2;
	cout << "================" << endl;
	cout << "Testing for repeating (matrix example):" << endl;
	cout << "================" << endl;
	tree2.insert("1010");
	tree2.insert("0000");
	tree2.insert("1010");
	tree2.insert("0101");
	tree2.print();

	cout << "================" << endl;
	cout << "Creating a Trie from a sentence and replacing superstrings with given substrings in a list:" << endl;
	cout << "================" << endl;
	cout << "Original Sentence: the cattle was rattled by the battery" << endl;
	list<string> z; z.push_back("cat"); z.push_back("bat"); z.push_back("rat"); z.push_back("t");
	printSent("the cattle was rattled by the battery", z);
	cout << "==========================" << endl;
	string stop; //pls don't disapear oh mighty command window
	cin >> stop;
}