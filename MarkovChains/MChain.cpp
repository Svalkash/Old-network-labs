#include "MChain.h"
#include <sstream>
#include <iomanip>
using namespace std;

MChain3::MChain3(double N, double MTTCA, double MTTBR, bool recover)
{
	stopAfterFail = !recover;
	//nulls
	IA = 0;
	timeLeft = 0;
	avgTUF = 0;
	reset();
	//probcalc
	prob[0][1] = N / MTTCA;
	prob[0][2] = 0;
	prob[0][0] = 1.0 - prob[0][1] - prob[0][2];
	prob[0][2] = 0;
	prob[1][0] = 1 / MTTBR;
	prob[1][2] = (N - 1.0) / MTTCA;
	prob[1][1] = 1.0 - prob[1][0] - prob[1][2];
	prob[2][1] = 0;
	prob[2][0] = recover ? 1 / MTTBR : 0;
	prob[2][2] = 1 - prob[2][0] - prob[2][1];
}

void MChain3::step()
{
	//check time
	if (timeLeft == 0)
		return;
	//gen
	double r = (double)rand() / (double)RAND_MAX;
	//time
	++currentTime;
	if (timeLeft > 0)
		--timeLeft;
	//select
	if (prob[state][0] >= r)
		state = 0;
	else
		if (prob[state][0] + prob[state][1] >= r)
			state = 1;
		else
			state = 2;
	//packet
	if (state == 2) {
		++transFailed;
		if (timeUntilFailure < 0)
			timeUntilFailure = currentTime;
	}
	else
		++transComplete;
}

void MChain3::reset()
{
	state = 0;
	transComplete = 0;
	transFailed = 0;
	currentTime = 0;
	timeUntilFailure = -1;
}

void MChain3::sim(int time)
{
	reset();
	timeLeft = time;
	while (timeLeft != 0) {
		step();
		if (stopAfterFail && timeUntilFailure > 0)
			break;
	}
	//sum to avg
	avgTUF += timeUntilFailure;
}

double MChain3::calcTUF(int num)
{
	//set
	for (int i = 0; i < num; ++i)
		sim();
	//avgcalc
	avgTUF /= num;
	return avgTUF;
}

double MChain3::calcIA(int time)
{
	sim(time);
	//calc IA
	IA = (double)transComplete / (double)time;
	return IA;
}

std::string MChain3::printProb()
{
	ostringstream oss;
	oss << '[';
	for (int i = 0; i < 3; ++i) {
		oss << '[';
		for (int j = 0; j < 3; ++j) {
			oss << setprecision(10) << prob[i][j];
			if (j < 2)
				oss << ',';
		}
		oss << ']';
		if (i < 2)
			oss << ",\n";
	}
	oss << ']';
	return oss.str();
}