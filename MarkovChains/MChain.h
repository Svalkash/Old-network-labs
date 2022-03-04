#pragma once
#include <string>

//template<int SIZE = 3>
class MChain3
{
private:
	int state;
	double prob[3][3];
	int transComplete;
	int transFailed;
	double IA;
	int timeLeft;
	int currentTime;
	double timeUntilFailure;
	bool stopAfterFail;
	//avg
	double avgTUF;
public:
	MChain3(double N, double MTTCA, double MTTBR, bool recover);
	void step();
	void reset();
	void sim(int time = -1);
	double calcTUF(int num);
	double calcIA(int time);
	
	std::string printProb();
};

