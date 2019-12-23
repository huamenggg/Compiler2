#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
using namespace std;

class CFG {
public:
	CFG();
	void OutputTokens(string line, ofstream &ofile);
	string Str2Token(string str);
	int NextState(int curState, char ch);
	string StateToString(int state, string str);
	string Ch2Type(char ch);
	string ExtractReservedWord(string str);

private:
	// DFA have 17 states
	vector<map<string, int>> mDFA;
};