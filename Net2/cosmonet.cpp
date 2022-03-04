#include "cosmonet.h"

Cosmonet::Cosmonet(): cur_status(), rp_status(), zv_status()
{
	for (int i = 0; i < PACK; i++) {
		zv_status[i] = 0;
		rp_status[i] = 0;
		cur_status[i] = 0;
	}
	resp_status = 0;
	credits = CRED_START;
	current_prob = 1.0;
	state = 0;
}

Cosmonet::Cosmonet(const Cosmonet& c) : cur_status(), rp_status(), zv_status()
{
	for (int i = 0; i < PACK; i++) {
		zv_status[i] = c.zv_status[i];
		rp_status[i] = c.rp_status[i];
		cur_status[i] = c.cur_status[i];
	}
	resp_status = c.resp_status;
	credits = c.credits;
	current_prob = c.current_prob;
	state = c.state;
}

double Cosmonet::calc_prob_good()
{
	if (state < 10) {
		//checking credits
		if (credits == 0)
			return 0.0;
		credits--;
		//sending
		cur_status[state] = 1;
		//sent, looking for next state
		int ns = state;
		while (ns < PACK && cur_status[ns])
			ns = (ns == PACK - 1) ? 10 : ns + 1;
		//creating subversions
		Cosmonet good(*this);
		good.current_prob = current_prob * (1 - FAIL_PROB);
		good.rp_status[state] = 1;
		good.state = ns;
		Cosmonet bad(*this);
		bad.current_prob = current_prob * FAIL_PROB;
		bad.rp_status[state] = 0;
		bad.state = ns;
		//return
		return good.calc_prob_good() + bad.calc_prob_good();
	}
	else {
		double gp = 0, bp = 0;
		//perfect response
		if (rp_good())
			gp = current_prob * (1 - FAIL_PROB);
		else {
			//good response
			Cosmonet good(*this);
			good.current_prob = current_prob * (1 - FAIL_PROB);
			//sync responses
			for (int i = 0; i < PACK; ++i) {
				good.zv_status[i] = rp_status[i];
				good.cur_status[i] = rp_status[i];
				good.resp_status = 1;
			}
			//sent, looking for next state
			good.state = 00;
			while (good.cur_status[good.state]) //no bound problems: checked it before
				good.state++;
			gp = good.calc_prob_good();
		}

		//bad response
		Cosmonet bad(*this);
		bad.current_prob = current_prob * FAIL_PROB;
		//sync responses
		for (int i = 0; i < PACK; ++i) {
			bad.rp_status[i] = zv_status[i];
			bad.cur_status[i] = zv_status[i];
			bad.resp_status = 0;
		}
		//sent, looking for next state
		bad.state = 00;
		while (bad.cur_status[bad.state]) //no bound problems: checked it before
			bad.state++;
		bp = bad.calc_prob_good();

		//ret
		return gp + bp;
	}
	return 0.0;
}

bool Cosmonet::rp_good() const
{
	for (int i = 0; i < PACK; ++i)
		if (!rp_status[i])
			return 0;
	return 1;
}
