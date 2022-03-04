#pragma once

const double FAIL_PROB = 0.145543489663;
const int PACK = 7;
const int CRED_START = 19;

class Cosmonet
{
public:
	bool zv_status[PACK];
	bool rp_status[PACK];
	bool cur_status[PACK];
	bool resp_status;
	int credits;
	int state;
	double current_prob;
	/*
	00-PACK - packet sending
	10 - response
	*/
	Cosmonet();
	Cosmonet(const Cosmonet& c);
	double calc_prob_good();
	bool rp_good() const;
};

