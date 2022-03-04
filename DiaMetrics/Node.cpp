
#include "Node.h"

bool Node::connect(Node& n) {
	con.insert(&n);
	n.con.insert(this);
	return 1; //assuming first equals to this
}