#include"CFG.h"

CFG::CFG() {
	ifstream ifile("CFG.txt");
	string line;
	bool isGrammar = false;
	bool isInitial = false;

	//增加增广文法
	SyntaxNode *extNode = new SyntaxNode;
	extNode->name = "_S";
	extNode->isTerminal = false;
	mNode.push_back(extNode);

	Grammar *extGrammar = new Grammar;
	extGrammar->head = 0;
	mGrammar.push_back(extGrammar);
	mNode[0]->grammar.push_back(0);

	while (getline(ifile, line)) {
		if (line == "%%") {
			isGrammar = true;
			continue;
		}
		vector<string> input;
		StringSplit(line, input, ":");

		if (!isGrammar) {
			bool isTerminal;
			if (input[0] == "Terminal") isTerminal = true;
			else isTerminal = false;

			vector<string> tokens;
			StringSplit(input[1], tokens, ",");

			for (int i = 0; i < tokens.size(); i++) {
				SyntaxNode *node = new SyntaxNode;
				node->isTerminal = isTerminal;
				node->name = tokens[i];
				mNode.push_back(node);
				if (!isInitial && !isTerminal) {
					isInitial = true;
					mGrammar[mNode[0]->grammar[0]]->child.push_back(mNode.size() - 1);
				}
			}
		}
		else {
			Grammar *grammar = new Grammar;
			grammar->head = FindNode(input[0]);

			vector<string> tokens;
			StringSplit(input[1], tokens, " ");

			for (int i = 0; i < tokens.size(); i++) {
				grammar->child.push_back(FindNode(tokens[i]));
			}
			mGrammar.push_back(grammar);
			mNode[grammar->head]->grammar.push_back(mGrammar.size() - 1);
		}
	}
	CreateItemFamily();
}

void CFG::CreateItemFamily() {

}
int CFG::FindNode(const string& s) {
	for (int i = 0; i < mNode.size(); i++) {
		if (mNode[i]->name == s) return i;
	}
	cout << "There is undefined Variable or Terminal in Grammar : " << s << endl;
	system("pause");
	exit(-1);
}
void CFG::PrintGrammar(int index) {
	cout << mNode[mGrammar[index]->head]->name << " : ";
	for (int i = 0; i < mGrammar[index]->child.size(); i++) {
		cout << mNode[mGrammar[index]->child[i]]->name << " ";
	}
	cout << endl;
}
void CFG::PrintAllGrammar() {
	for (int i = 0; i < mGrammar.size(); i++) PrintGrammar(i);
}
void CFG::StringSplit(const string& s, vector<string>& tokens, const string& delimiters)
{
	string::size_type lastPos = s.find_first_not_of(delimiters, 0);
	string::size_type pos = s.find_first_of(delimiters, lastPos);
	while (string::npos != pos || string::npos != lastPos) {
		tokens.push_back(s.substr(lastPos, pos - lastPos));
		lastPos = s.find_first_not_of(delimiters, pos);
		pos = s.find_first_of(delimiters, lastPos);
	}
}