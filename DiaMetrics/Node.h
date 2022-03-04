#pragma once

#include <set>

class Node
{
public:
	int num;
	std::multiset<Node*> con;
	Node() :num(0) {};
	Node(const int a) :num(a) {};
	bool connect(Node& n);
	operator int() const { return num; }
	//bool operator==(const Node& n) const { return n.add == add; }
};