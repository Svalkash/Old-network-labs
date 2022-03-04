#pragma once

#include <map>
#include <set>


class Node
{
public:
	std::map<int, double> cons;
	double probTo50;

	Node() : probTo50(-1.0) {}
};

