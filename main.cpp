#include"DFA2Token.h"
#include"CFG.h"
using namespace std;

int main() {
	string line;
	DFA dfa;
	CFG cfg;
	string inputFileName;
	vector<InputNode*> inputSequence;
#ifndef DEBUG
	cout << "Please input the program file name:" << endl;
	getline(cin, inputFileName);
	ifstream ifile(inputFileName, ios::in);
#else
	ifstream ifile("input.txt");
#endif
	ofstream reductionFile("reduction.txt");
	if (!ifile) {
		cout << "File \"" << inputFileName << "\" can't open" << endl;
		system("pause");
		exit(-1);
	}
	if (!reductionFile) {
		cout << "File reduction.txt create failed" << endl;
		system("pause");
		exit(-1);
	}
	while (getline(ifile, line)) {
		dfa.OutputTokens(line, inputSequence, cfg);
	}
	InputNode *newNode = new InputNode;
	newNode->content = "";
	newNode->index = cfg.FindNode("$");
	inputSequence.push_back(newNode);
	cfg.Run(inputSequence, reductionFile);
	ifile.close();
	reductionFile.close();
	system("pause");
	return 0;
}