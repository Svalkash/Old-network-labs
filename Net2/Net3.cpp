#include <iostream>
#include <fstream>
#include "net3func.h"
using namespace std;

string task_list[] = { "1. Hamming - decode",
"2. Hamming - encode",
"3. FCS calc",
"4. Cat" };

int main()
{
	string ifname, ofname;
	int tasknum;
	for (int i = 0; i < 4; i++)
		cout << task_list[i] << endl;
	cout << "Task number: ";
	cin >> tasknum;
	/*
	ifname = "input_";
	ifname += char(tasknum + 48);
	ifname += ".txt";
	ofname = "output_";
	ofname += char(tasknum + 48);
	ofname += ".txt";
	//opening
	ifstream ifs(ifname);
	ofstream ofs(ofname);
	*/
	switch (tasknum)
	{
	case 1:
	{
		cout << hamming_decode(1, 1, 1) << endl;

		/*
		cout << hamming_decode(0, 0, 0) << endl;
		cout << hamming_decode(0, 0, 1) << endl;
		cout << hamming_decode(0, 1, 0) << endl;
		cout << hamming_decode(0, 1, 1) << endl;
		cout << hamming_decode(1, 0, 0) << endl;
		cout << hamming_decode(1, 0, 1) << endl;
		cout << hamming_decode(1, 1, 0) << endl;
		cout << hamming_decode(1, 1, 1) << endl;
		*/
		break;
	}
	case 2:
	{
		int* octs = hamming_encode(1, 1, 1);
		for (int i = 0; i < ENC_BYTES; ++i)
			cout << octs[i] << ", ";
		cout << endl;
		break;
	}
	case 3:
	{
		cout << fcs() << endl /*<< fcs_1() << endl*/;		
		break;
	}
	case 4:
	{
		cout << cat();
		break;
	}
	default:
		cerr << "wrong_action";
		break;
	}
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
