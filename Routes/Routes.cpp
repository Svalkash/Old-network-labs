#include <iostream>
#include <fstream>
#include "rfunc.h"
using namespace std;

string task_list[] = { "1. Easy",
"2. Medium" };

int main()
{
	int tasknum;
	for (int i = 0; i < 2; i++)
		cout << task_list[i] << endl;
	cout << "Task number: ";
	cin >> tasknum;
	switch (tasknum)
	{
	case 1:
	{
		cout << rEasy();
		break;
	}
	case 2:
	{
		cout << rMedium();
		break;
	}
	default:
		cerr << "wrong_action";
		break;
	}
	return 0;
}