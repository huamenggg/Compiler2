#include"CFG.h"

CFG::CFG(void) {
	ifstream ifile("CFG.txt");
	string line;
	bool isGrammar = false;
	bool isInitial = false;
	mNode.clear();
	mGrammar.clear();
	mItemFamily.clear();

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

	SyntaxNode *endSymbol = new SyntaxNode;
	endSymbol->name = "$";
	endSymbol->isTerminal = true;
	mNode.push_back(endSymbol);

	for (int i = 0; i < mNode.size(); i++)CalcFirst(i);
	InputFollow();

	CreateItemFamily();
	cout << endl;
}
void CFG::InputFollow(void) {
	ifstream ifile("follow.txt");
	string line;

	while (getline(ifile, line)) {
		vector<string> input;
		StringSplit(line, input, ":");
		int head = FindNode(input[0]);

		vector<string> tokens;
		StringSplit(input[1], tokens, " ");

		for (int i = 0; i < tokens.size(); i++) {
			mNode[head]->follow.push_back(FindNode(tokens[i]));
		}
	}
}
void CFG::CalcFirst(const int index) {
	if (mNode[index]->isTerminal) {
		mNode[index]->first.push_back(index);
	}
	else {
		for (int i = 0; i < mNode[index]->grammar.size(); i++) {
			vector<int>& child = mGrammar[mNode[index]->grammar[i]]->child;
			if(!IsFirstContain(index, child[0]))
				mNode[index]->first.push_back(child[0]);
		}
	}
}
bool CFG::IsFirstContain(const int index, const int target) {
	vector<int> &first = mNode[index]->first;
	for (int i = 0; i < first.size(); i++) {
		if (first[i] == target) return true;
	}
	return false;
}
bool SortItem(ItemNode* a, ItemNode* b) {
	return a->grammar < b->grammar;
}
bool CFG::CanAddMoreItem(void) {
	//TODO: need to judge if can add more
	// because the time complexity will be high
	// and generally, one time will be enough
	return true;
}
void CFG::CLOSURE(Item *item) {
	vector<int> nextsymbol;
	do {
		for (int i = 0; i < item->itemNode.size(); i++) {
			int index = item->itemNode[i]->index;
			int grammarIndex = item->itemNode[i]->grammar;
			if (index >= mGrammar[grammarIndex]->child.size())continue;
			int next = index + 1;
			nextsymbol.clear();
			if (next >= mGrammar[grammarIndex]->child.size()) {
				nextsymbol = item->itemNode[i]->symbol;
			}
			else {
				nextsymbol = mNode[mGrammar[grammarIndex]->child[next]]->first;
			}
			sort(nextsymbol.begin(), nextsymbol.end());
			int nodeIndex = mGrammar[grammarIndex]->child[index];
			if (IsItemNodeContain(nodeIndex, item)) continue;
			vector<int>& grammar = mNode[nodeIndex]->grammar;
			for (int j = 0; j < grammar.size(); j++) {
				ItemNode *newNode = new ItemNode;
				newNode->grammar = grammar[j];
				newNode->index = 0;
				newNode->symbol = nextsymbol;
				item->itemNode.push_back(newNode);
			}
		}
	} while (!CanAddMoreItem());
	sort(item->itemNode.begin(), item->itemNode.end(), SortItem);
}
bool CFG::IsItemNodeContain(const int index, Item* item) {
	for (int i = 0; i < item->itemNode.size(); i++) {
		if (index == mGrammar[item->itemNode[i]->grammar]->head && item->itemNode[i]->index == 0) return true;
	}
	return false;
}
void CFG::GOTO(const int itemIndex, const int nodeIndex) {
	Item *newItem = new Item;
	newItem->itemNode.clear();

	vector<ItemNode*>& item = mItemFamily[itemIndex]->itemNode;
	for (int i = 0; i < item.size(); i++) {
		if (item[i]->index >= mGrammar[item[i]->grammar]->child.size())continue;
		if (mGrammar[item[i]->grammar]->child[item[i]->index] == nodeIndex) {
			ItemNode * newNode = new ItemNode;
			newNode->grammar = item[i]->grammar;
			newNode->index = item[i]->index + 1;
			newNode->symbol = item[i]->symbol;
			newItem->itemNode.push_back(newNode);
		}
	}
	if (newItem->itemNode.size() == 0) return;
	CLOSURE(newItem);
	if (!IsItemFamilyContain(newItem)) mItemFamily.push_back(newItem);
	GotoNode *newGoto = new GotoNode;
	newGoto->next = mItemFamily.size() - 1;
	newGoto->symbol = nodeIndex;
	mItemFamily[itemIndex]->gotoList.push_back(newGoto);
}
bool CFG::IsItemFamilyContain(Item* newItem) {
	//Need to check
	bool flag = false;
	vector<ItemNode*>& target = newItem->itemNode;
	for (int i = 0; i < mItemFamily.size(); i++) {
		vector<ItemNode*>& source = mItemFamily[i]->itemNode;
		if (source.size() != newItem->itemNode.size()) continue;
		for (int j = 0; j < target.size(); j++) {
			// because the itemNode has been sorted by grammar index, so
			// if grammar is not equal, then the two sets are noe equal
			if (source[j]->grammar != target[j]->grammar) goto L1;
			if (source[j]->index != target[j]->index) goto L1;
			if (source[j]->symbol.size() != target[j]->symbol.size()) goto L1;
			for (int k = 0; k < target[j]->symbol.size(); k++) {
				if (source[j]->symbol[k] != target[j]->symbol[k]) goto L1;
			}
		}
		// There is a item every grammar, index and symbol is equal
		// to new item, so repeat.
		flag = true;
	L1:continue;
	}
	return flag;
}
void CFG::CreateItemFamily() {
	ItemNode *start = new ItemNode;
	start->grammar = 0;
	start->index = 0;
	start->symbol.push_back(FindNode("$"));

	Item *I0 = new Item;
	I0->itemNode.push_back(start);
	CLOSURE(I0);
	mItemFamily.push_back(I0);

	do {
		for (int i = 0; i < mItemFamily.size(); i++) {
			for (int j = 0; j < mNode.size() - 1; j++) {
				GOTO(i, j);
			}
		}
		//TODO: check if can be added more
	} while (false);
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