#include"DFA2Token.h"

DFA::DFA() : mDFA(17){
	//initial state
	mDFA[0].insert(pair<string, int>("_", 1));
	mDFA[0].insert(pair<string, int>("L", 1));
	mDFA[0].insert(pair<string, int>("D", 2));
	mDFA[0].insert(pair<string, int>(">", 3));
	mDFA[0].insert(pair<string, int>("<", 3));
	mDFA[0].insert(pair<string, int>("!", 3));
	mDFA[0].insert(pair<string, int>("+", 5));
	mDFA[0].insert(pair<string, int>("-", 6));
	mDFA[0].insert(pair<string, int>("*", 7));
	mDFA[0].insert(pair<string, int>("/", 8));
	mDFA[0].insert(pair<string, int>("(", 9));
	mDFA[0].insert(pair<string, int>(")", 10));
	mDFA[0].insert(pair<string, int>("}", 11));
	mDFA[0].insert(pair<string, int>("{", 12));
	mDFA[0].insert(pair<string, int>(";", 13));
	mDFA[0].insert(pair<string, int>(",", 14));
	mDFA[0].insert(pair<string, int>("=", 15));

	mDFA[1].insert(pair<string, int>("_", 1));
	mDFA[1].insert(pair<string, int>("L", 1));
	mDFA[1].insert(pair<string, int>("D", 1));

	mDFA[2].insert(pair<string, int>("D", 2));

	mDFA[3].insert(pair<string, int>("=", 4));

	mDFA[15].insert(pair<string, int>("=", 16));
}

int DFA::NextState(int curState, char ch) {
	int nextState = -1;
	string curStr = Ch2Type(ch);
	map<string, int>::iterator iter;
	iter = mDFA[curState].begin();
	while (iter != mDFA[curState].end()) {
		if (iter->first == curStr) {
			nextState = iter->second;
			break;
		}
		iter++;
	}
	return nextState;
}

string DFA::Ch2Type(char ch) {
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) return "L";
	if (ch >= '0' && ch <= '9') return "D";
	return string(1, ch);
}

void DFA::OutputTokens(string line, vector<InputNode*> &inputSequence, CFG& cfg) {
	string curType;
	string nextType;
	int begin, end;
	for (begin = 0, end = 1; end < line.length();) {
		//current char is ' ', ignore
  		if (line[begin] == ' ' || line[begin] == '\t') {
			begin++; end++; continue;
		}

		// if is relop
		if (line[begin] == '<' || line[begin] == '>'
			|| line[begin] == '!' || line[begin] == '=') {
			if (line[end] == '=') {
				InputNode *newNode = new InputNode;
				newNode->index = cfg.FindNode(Str2Token(line.substr(begin, end - begin + 1)));
				newNode->content = line.substr(begin, end - begin + 1);
				inputSequence.push_back(newNode);
				begin = end + 1;
				end += 2;
				continue;
			}
		}

		//current char can't be part of ID or Number, output
		curType = Ch2Type(line[begin]);
		if (curType != "L" && curType != "D" && curType != "_") {
			InputNode *newNode = new InputNode;
			newNode->index = cfg.FindNode(Str2Token(line.substr(begin, 1)));
			newNode->content = line.substr(begin, 1);
			inputSequence.push_back(newNode);
			begin++; end++; continue;
		}

		//end is ' ', output before string
		if (line[end] == ' ' || line[end] == '\t') {
			InputNode *newNode = new InputNode;
			newNode->index = cfg.FindNode(Str2Token(line.substr(begin, end - begin)));
			newNode->content = line.substr(begin, end - begin);
			inputSequence.push_back(newNode);
			begin = end + 1;
			end += 2;
			continue;
		}

		nextType = Ch2Type(line[end]);
		if (nextType != "L" && nextType != "D" && nextType != "_") {
			InputNode *newNode = new InputNode;
			newNode->index = cfg.FindNode(Str2Token(line.substr(begin, end - begin)));
			newNode->content = line.substr(begin, end - begin);
			inputSequence.push_back(newNode);

			newNode = new InputNode;
			newNode->index = cfg.FindNode(Str2Token(line.substr(end, 1)));
			newNode->content = line.substr(end, 1);
			inputSequence.push_back(newNode);
			begin = end + 1;
			end += 2;
			continue;
		}

		end++;
	}
	if (begin < line.length()) {
		InputNode *newNode = new InputNode;
		newNode->index = cfg.FindNode(Str2Token(line.substr(begin, end - begin)));
		newNode->content = line.substr(begin, end - begin);
		inputSequence.push_back(newNode);
	}
}

string DFA::Str2Token(string str) {
	int state = 0; // initial state
	for (int i = 0; i < str.length(); i++) {
		state = NextState(state, str[i]);
		if (state == -1) break;
	}
	return StateToString(state, str);
}

string DFA::ExtractReservedWord(string str) {
	switch (str.length())
	{
	case 2: if (str == "if") return "IF";
		break;
	case 3: if (str == "int") return "INT";
		break;
	case 4: if (str == "else") return "ELSE";
		break;
	case 5: if (str == "while") return "WHILE";
		break;
	case 6: if (str == "return") return "RETURN";
		break;
	default:
		return "ID";
	}
	return "ID";
}

string DFA::StateToString(int state, string str) {
	switch (state)
	{
	case -1: return "Error";
	case 1: return ExtractReservedWord(str);
	case 2: return "NUMBER";
	case 3: return "RELOP";
	case 4: return "RELOP";
	case 5: return "PLUS";
	case 6: return "MINUS";
	case 7: return "STAR";
	case 8: return "DIV";
	case 9: return "LP";
	case 10: return "RP";
	case 11: return "LC";
	case 12: return "RC";
	case 13: return "SEMI";
	case 14: return "COMMA";
	case 15: return "ASSIGNOP";
	case 16: return "RELOP";
	default:
		return "Error";
		break;
	}
}