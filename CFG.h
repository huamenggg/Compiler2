#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
using namespace std;

struct Grammar {
	int head;
	vector<int> child;
};

struct SyntaxNode {
	string name;
	vector<Grammar> g;
	bool isTerminal;
};

class CFG {
public:
	CFG();

private:
	vector<SyntaxNode> mNode;
	vector<Grammar> mGrammar;
};