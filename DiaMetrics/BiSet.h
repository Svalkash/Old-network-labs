#pragma once

#include "Node.h"
#include <set>


class BiSet
{
public:
	std::set<Node*> S;
	bool insert(Node* n) { return S.insert(n).second; }
	BiSet& operator+=(Node* n) { insert(n); return *this; };
	BiSet& operator=(const BiSet& bs) { S = bs.S; return *this; };
	int thicc(Node* n) const;
};

