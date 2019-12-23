#include"CFG.h"
using namespace std;

int main() {
	string line;
	CFG cfg;
	string inputFileName;
	cout << "Please input the program file name:" << endl;
	//getline(cin, inputFileName);
	//ifstream ifile(inputFileName, ios::in);
	ifstream ifile("input.txt", ios::in);
	ofstream ofile("result.txt", ios::out);
	if (!ifile) {
		cout << "File \"" << inputFileName << "\" can't open" << endl;
		system("pause");
		exit(-1);
	}
	if (!ofile) {
		cout << "File result.txt create failed" << endl;
		system("pause");
		exit(-1);
	}
	while (getline(ifile, line)) {
		ofile << endl;
	}
	ifile.close();
	ofile.close();
	system("pause");
	return 0;
}