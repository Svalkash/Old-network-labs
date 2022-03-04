#include "rfunc.h"
#include "Node.h"
#include <cmath>

int RCount[N + 1][M + 1];

int rEasy()
{
	//filling all to 0
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
			RCount[i][j] = 0;
	//vertical and horizontal
	for (int i = 1; i <= N; ++i)
		RCount[i][1] = 1;
	for (int j = 1; j <= M; ++j)
		RCount[1][j] = 1;
	//count
	return finDeWae(N, M);
}

int finDeWae(int x, int y)
{
	if (RCount[x][y] == 0)
		RCount[x][y] = finDeWae(x - 1, y) + finDeWae(x, y - 1) + finDeWae(x - 1, y - 1);
	return RCount[x][y];
}


const int NodeNUM = 50;
const int ConNUM = 111;

int cons[ConNUM][2] = { {1, 2}, {1, 3}, {1, 6}, {1, 8}, {1, 11}, {1, 15}, {1, 18}, {1, 38}, {1, 40}, {2, 4}, {2, 5}, {2, 7}, {2, 25}, {3, 17}, {3, 19}, {4, 9}, {4, 16}, {4, 21}, {4, 22}, {5, 13}, {5, 19}, {5, 25}, {6, 10}, {6, 28}, {7, 12}, {7, 14}, {7, 27}, {8, 14}, {8, 27}, {9, 12}, {9, 19}, {10, 13}, {10, 32}, {10, 34}, {11, 23}, {12, 18}, {12, 22}, {12, 23}, {13, 27}, {13, 36}, {14, 16}, {14, 32}, {14, 33}, {15, 24}, {15, 27}, {15, 28}, {15, 29}, {15, 30}, {16, 18}, {16, 20}, {16, 28}, {17, 39}, {17, 46}, {18, 19}, {19, 24}, {20, 25}, {20, 26}, {20, 37}, {20, 44}, {21, 24}, {21, 35}, {22, 31}, {22, 33}, {22, 37}, {23, 32}, {23, 37}, {23, 39}, {24, 28}, {25, 34}, {25, 48}, {26, 31}, {26, 35}, {26, 50}, {27, 50}, {28, 50}, {29, 45}, {29, 47}, {30, 43}, {30, 48}, {30, 49}, {31, 39}, {32, 36}, {33, 35}, {33, 39}, {33, 49}, {34, 42}, {35, 43}, {36, 50}, {37, 45}, {37, 48}, {38, 44}, {38, 45}, {38, 46}, {39, 42}, {40, 41}, {40, 42}, {40, 44}, {40, 46}, {41, 42}, {41, 45}, {41, 48}, {42, 48}, {43, 44}, {43, 47}, {44, 49}, {45, 50}, {46, 47}, {46, 50}, {47, 50}, {48, 50}, {49, 50} };

int scans[ConNUM] = { 865, 795, 1830, 332, 1144, 1854, 757, 1482, 273, 1030, 1865, 881, 1019, 759, 740, 1611, 1881, 497, 1862, 1316, 781, 431, 570, 502, 1787, 1435, 707, 1724, 1315, 394, 1950, 1662, 1275, 1196, 332, 290, 1118, 846, 1159, 1335, 407, 1247, 1350, 395, 1386, 1564, 1761, 713, 1417, 1018, 1754, 1885, 1708, 1541, 1368, 1490, 1989, 484, 940, 1780, 1343, 261, 1790, 1283, 1655, 1730, 357, 987, 1410, 291, 940, 1524, 1738, 656, 1269, 873, 498, 454, 456, 727, 1101, 1045, 378, 768, 1841, 274, 416, 379, 502, 1803, 1150, 1369, 483, 1804, 1269, 1118, 1677, 1816, 502, 1193, 1451, 496, 1526, 885, 861, 377, 1874, 641, 962, 1342, 996 };

const double scanTime = 0.4335 * 0.001;

Node Net[NodeNUM + 1];



void init()
{
	for (int i = 0; i < ConNUM; ++i)
		Net[cons[i][0]].cons.insert(std::make_pair(cons[i][1], initProb(i)));
	Net[50].probTo50 = 1;
}

double initProb(int n)
{
	return exp(-scans[n] * scanTime);
}

double calcProbs(int nodeNum)
{
	if (Net[nodeNum].probTo50 > 0)
		return Net[nodeNum].probTo50;
	//calc
	double prob = 0;

	//sum with new coeffs
	/*
	for (auto it = Net[nodeNum].cons.begin(); it != Net[nodeNum].cons.end(); it++)
		prob += calcProbs((*it).first) * (*it).second;
	//div by num
	prob /= Net[nodeNum].cons.size();
	*/

	//select max
	for (auto it = Net[nodeNum].cons.begin(); it != Net[nodeNum].cons.end(); it++)
		if (calcProbs((*it).first) * (*it).second > prob)
			prob = calcProbs((*it).first) * (*it).second;
	//ret
	return Net[nodeNum].probTo50 = prob;
}

double rMedium()
{
	init();
	return calcProbs(1);
}