#pragma once

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>
#include"DFA2Token.h"
using namespace std;

struct InputNode;

struct SyntaxNode {
	//SyntaxNode name, such as "Exp","SEMI"
	string name;
	//The grammars index of this SyntaxNode, such as "Exp->Exp PLUS Exp"
	//This can save time of searching for grammar in mGrammar
	vector<int> grammar;
	//If isTerminal is TRUE, then this node is terminal
	bool isTerminal;
	//The index of the FIRST element of this node
	vector<int> first;
};

struct Grammar {
	//For example: Exp->Exp PLUS Exp
	//head is the index in mNode of "Exp"
	int head;
	//child is a list of indexs in mNode of "Exp" "PLUS" "Exp"
	vector<int> child;
};

struct ItemNode {
	//For example: [_S->.Stmt, $](start grammar)
	//The index in mGrammar of this Grammar: 0
	int grammar;
	//The location of '.' : 0
	int index;
	//The list of the next symbol: "$"
	vector<int> symbol;
};

struct GotoNode {
	//The index in mNode of symbol
	int symbol;
	//Next ItemNode
	int next;
};

struct Item {
	vector<ItemNode*> itemNode;
	vector<GotoNode*> gotoList;
};

class CFG {
public:
	CFG(void);
	void StringSplit(const string& s, vector<string>& tokens, const string& delimiters);
	int FindNode(const string& s);
	void PrintGrammar(int index, ofstream& ofile);
	void CreateItemFamily(void);
	void CalcFirst(const int index);
	void CLOSURE(Item *item);
	void GOTO(const int itemIndex, const int nodeIndex);
	void Run(vector<InputNode*>& inputSequence, ofstream& ofile);
	void ModifyFirst(const int index);
	int IsItemFamilyContain(Item* newItem);
	bool IsFirstContain(const int index, const int target);
	bool CanAddMoreItem(const int num);
	bool IsItemNodeContain(const int index, Item* item, vector<int>& nextsymbol);
	bool IsFirstContain(vector<int>& first, const int index);
	bool IsSymbolContain(const int index, const int target, Item* item);

private:
	vector<SyntaxNode*> mNode;
	vector<Grammar*> mGrammar;
	vector<Item*> mItemFamily;
};