#include "LenTable.h"

void LenTable::reset(std::set<Node*> nlist)
{
	size = nlist.size();
	mat.clear();
	for (auto it = nlist.begin(); it != nlist.end(); it++)
		for (auto it2 = nlist.begin(); it2 != nlist.end(); it2++)
			mat.insert(std::make_pair(std::make_pair((*it)->num, (*it2)->num), (it == it2) ? 0 : INT_MAX));
}
