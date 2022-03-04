// MarkovChains.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <ctime>
#include "MChain.h"
using namespace std;

int N = 4;
int MTTCA = 426;
int MTTBR = 7;

int main()
{
    srand(time(nullptr));
    int canRecover;
    cout << "Select test (0 = w/out recover, 1 = with recover): ";
    cin >> canRecover;
    MChain3 MC(N, MTTCA, MTTBR, canRecover);
    cout << "Probabilities:" << endl << MC.printProb() << endl;
    if (!canRecover) {
        int iter;
        cout << "Select iter number: ";
        cin >> iter;
        cout << "Avg TUF = " << MC.calcTUF(iter) << endl;
    }
    else {
        int testTime;
        cout << "Select time: ";
        cin >> testTime;
        cout << "Avg IA = " << MC.calcIA(testTime) << endl;
    }
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
