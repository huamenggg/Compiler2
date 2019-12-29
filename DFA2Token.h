#pragma once
#define DEBUG
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include"CFG.h"
using namespace std;

struct InputNode {
	string content;
	int index;
};
class CFG;

class DFA {
public:
	DFA();
	void OutputTokens(string line, vector<InputNode*>& inputSequence, CFG& cfg);
	string Str2Token(string str);
	int NextState(int curState, char ch);
	string StateToString(int state, string str);
	string Ch2Type(char ch);
	string ExtractReservedWord(string str);

private:
	// DFA have 17 states
	vector<map<string, int>> mDFA;
};