#pragma once

#include <map>
#include <set>
#include "Node.h"

class LenTable
{
private:
	int size;
	std::map<std::pair<int, int>, int> mat;
public:
	LenTable() :size(0) {};
	void reset(std::set<Node*> nlist);
	int& operator()(int i, int j) { return mat.at(std::make_pair(i, j)); };
};

