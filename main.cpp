#include"DFA2Token.h"
using namespace std;

int main() {
	string line;
	DFA dfa;
	string inputFileName;
#ifndef DEBUG
	cout << "Please input the program file name:" << endl;
	getline(cin, inputFileName);
	ifstream ifile(inputFileName, ios::in);
#else
	ifstream ifile("input.txt");
#endif
	ofstream tokenFile("token.txt");
	ofstream reductionFile("reduction.txt");
	if (!ifile) {
		cout << "File \"" << inputFileName << "\" can't open" << endl;
		system("pause");
		exit(-1);
	}
	if (!tokenFile) {
		cout << "File token.txt create failed" << endl;
		system("pause");
		exit(-1);
	}
	if (!reductionFile) {
		cout << "File reduction.txt create failed" << endl;
		system("pause");
		exit(-1);
	}
	while (getline(ifile, line)) {
		dfa.OutputTokens(line, tokenFile);
		tokenFile << '\t';
	}
	ifile.close();
	tokenFile.close();
	ifile.open("token.txt");
	reductionFile.close();
	system("pause");
	return 0;
}