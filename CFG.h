#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>
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
	vector<int> first;
	vector<int> follow;
};

struct ItemNode {
	int grammar;
	int index;
	vector<int> symbol;
};

struct GotoNode {
	int symbol;
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
	void PrintGrammar(int index);
	void PrintAllGrammar(void);
	void CreateItemFamily(void);
	void CalcFirst(const int index);
	void InputFollow(void);
	bool IsFirstContain(const int index, const int target);
	void CLOSURE(Item *item);
	void GOTO(const int itemIndex, const int nodeIndex);
	bool IsItemFamilyContain(Item* newItem);
	bool CanAddMoreItem(void);
	bool IsItemNodeContain(const int index, Item* item);

private:
	vector<SyntaxNode*> mNode;
	vector<Grammar*> mGrammar;
	vector<Item*> mItemFamily;
};