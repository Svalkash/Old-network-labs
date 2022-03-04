// DiaMetrics.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include "Net.h"
using namespace std;

string task_list[] = { "1. Diameter (no cycles).",
"2. Diameter (any)",
"3. Connectivity"};

int main()
{
	string ifname;
	int tasknum;
	for (int i = 0; i < 3; i++)
		cout << task_list[i] << endl;
	cout << "Task number: ";
	cin >> tasknum;
	ifname = "input_";
	ifname += char(tasknum + 48);
	ifname += ".txt";
	//opening
	ifstream ifs(ifname);
	if (!ifs)
	{
		cerr << "cannot_open_file" << endl;
		return 1;
	}
	switch (tasknum)
	{
	case 1:
	{
		Net N(ifs);
		int d = N.diam_nocycles();
		cout << d << endl;
		break;
	}
	case 2:
	{
		Net N(ifs);
		int d = N.diam();
		cout << d << endl;
		break;
	}
	case 3:
	{
		Net N(ifs);
		int c = N.connectivity();
		cout << c << endl;
		break;
	}
	default:
		cerr << "wrong_action";
		break;
	}
	ifs.close();
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
