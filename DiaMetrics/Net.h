#pragma once

#include <map>
#include "Node.h"
#include "LenTable.h"
#include "BiSet.h"

class Net
{
private:
	std::set<Node*> nlist;
	LenTable len;
public:
	Net(std::ifstream&);
	bool addNode(const int add);
	bool addLink(const int fr, const int to);
	Node* find(const int add) const;

	int diam_nocycles();
	int diam();

	int connectivity();

	int curCon(BiSet bs);

	~Net();
};

