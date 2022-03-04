#include <string>
#include <iostream>
#include <fstream>
#include <queue>
#include "Net.h"

using namespace std;

Net::Net(std::ifstream& ifs)
{
	char c;
	//trans
	ifs.get(c);
	if (c != '[')
		throw exception("wrong_input");
	while (c != '.') //[..], [..],..
	{
		ifs.get(c);
		if (c == '[')
		{
			int fr; //2, 3],...
			int to;
			ifs >> fr;
			ifs.get(c);
			ifs.get(c);
			ifs >> to;
			addNode(fr);
			addNode(to);
			addLink(fr, to);
		}
	}
}

bool Net::addNode(const int add)
{
	//test if op works
	bool flag = 1;
	for (auto it = nlist.cbegin(); it != nlist.cend(); it++)
		if (**it == add)
			return 0;
	auto t = new Node(add);
	nlist.insert(t);
	return flag;
}

bool Net::addLink(const int fr, const int to)
{
	if (!find(fr) || !find(to))
		throw runtime_error("No such address");
	return find(fr)->connect(*find(to));
}

Node* Net::find(const int add) const
{
	for (auto it = nlist.begin(); it != nlist.end(); it++)
		if (**it == add)
			return *it;
	return nullptr;
}

int Net::diam_nocycles()
{
	//create size table
	len.reset(nlist);
	//start queue
	queue<Node*> Q;
	//add process-queue to Node
	Q.push(*nlist.begin());
	while (Q.size())
	{
		Node* n = Q.front();
		//count
		for (auto it = n->con.begin(); it != n->con.end(); it++)
			if (len(n->num, (*it)->num) == INT_MAX) {
				len(n->num, (*it)->num) = 1;
				for (auto it2 = nlist.begin(); it2 != nlist.end(); it2++)
					if (len(n->num, (*it2)->num) + 1 < len((*it)->num, (*it2)->num) && len(n->num, (*it2)->num) != INT_MAX)
						len((*it)->num, (*it2)->num) = len((*it2)->num, (*it)->num) = len(n->num, (*it2)->num) + 1;
				//as we use Queue, we ALWAYS add new nodes to it
				Q.push(*it);
			}
		Q.pop();
	}
	//collect results
	int d = 0;
	for (int i = 0; i < nlist.size(); ++i)
		for (int j = 0; j < nlist.size(); ++j)
			if (len(i, j) > d)
				d = len(i, j);
	if (d == INT_MAX)
		throw runtime_error("There should be no more INTMAXes");
	//return
	return d;
}

int Net::diam()
{
	//create size table
	len.reset(nlist);
	//find lens for each node
	for (auto it = nlist.begin(); it != nlist.end(); it++) {
		Node* s = (*it);
		//start queue
		queue<Node*> Q;
		//add process-queue to Node
		Q.push(s);
		while (Q.size())
		{
			Node* n = Q.front();
			//count
			for (auto it = n->con.begin(); it != n->con.end(); it++)
				if (len(s->num, n->num) + 1 < len(s->num, (*it)->num)) {
					if (len(s->num, (*it)->num) != INT_MAX)
						throw runtime_error("Unexpected queue order, no INTMAX here((");
					len(s->num, (*it)->num) = len(s->num, n->num) + 1;
					//as we use Queue, we ALWAYS add new nodes to it
					Q.push(*it);
				}
			Q.pop();
		}
	}
	//collect results
	int d = 0;
	for (int i = 0; i < nlist.size(); ++i)
		for (int j = 0; j < nlist.size(); ++j)
			if (len(i, j) > d)
				d = len(i, j);
	if (d == INT_MAX)
		throw runtime_error("There should be no more INTMAXes");
	//return
	return d;
}

int Net::connectivity()
{
	BiSet BS;
	//Select first node
	BS += *nlist.begin();
	int conRes = INT_MAX;
	while (BS.S.size() < nlist.size()) {
		int minT = INT_MAX, maxT = 0, curCon = 0;
		Node* superThicc = nullptr;
		for (auto it = nlist.cbegin(); it != nlist.cend(); it++)
			if (BS.S.find(*it) == BS.S.end()) {
				int curT = BS.thicc(*it);
				curCon += curT;
				if (curT > maxT) {
					maxT = curT;
					superThicc = *it;
				}
			}
		if (curCon < conRes)
			conRes = curCon;
		BS += superThicc;
	}
	return conRes;
}

int Net::curCon(BiSet bs)
{
	int cons = 0;
	for (auto bsit = bs.S.begin(); bsit != bs.S.end(); bsit++)
		for (auto cit = (*bsit)->con.begin(); cit != (*bsit)->con.end(); cit++)
			if (bs.S.find(*cit) == bs.S.end())
				cons++;
	return cons;
}

Net::~Net()
{
	for (auto it = nlist.begin(); it != nlist.end(); it++)
		delete *it;
}