#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include"DFA2Token.h"
using namespace std;

struct Grammar {
	int head;
	vector<int> child;
};

struct SyntaxNode {
	string name;
	vector<int> grammar;
	bool isTerminal;
};

class CFG {
public:
	CFG();
	void StringSplit(const string& s, vector<string>& tokens, const string& delimiters);
	int FindNode(const string& s);
	void PrintGrammar(int index);
	void PrintAllGrammar();
	void CreateItemFamily();

private:
	vector<SyntaxNode*> mNode;
	vector<Grammar*> mGrammar;
};