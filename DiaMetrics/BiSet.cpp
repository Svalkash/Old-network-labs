#include "BiSet.h"

int BiSet::thicc(Node* n) const
{
	int tckn = 0;
	for (auto it = n->con.begin(); it != n->con.end(); it++)
		if (S.find(*it) != S.end())
			tckn++;
	return tckn;
}
