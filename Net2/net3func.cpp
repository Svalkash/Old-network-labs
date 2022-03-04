#include "net3func.h"
#include <iostream>
#include <sstream>
#include <vector>

bool get_bit(uint64_t i, int b)
{
	return (i & (uint64_t(1) << b)) >> b;
}

uint64_t &set_bit(uint64_t& i, int b, bool t)
{
	if (get_bit(i, b) && !t)
		i =  i - (uint64_t(1) << b);
	if (!get_bit(i, b) && t)
		i = i + (uint64_t(1) << b);
	return i;
}

uint64_t &inv_bit(uint64_t& i, int b)
{
	return set_bit(i, b, !get_bit(i, b));
}

uint64_t& add_bit(uint64_t& i, int b)
{
	i = (i << 1) + b;
	return i;
}

uint64_t& clear_bit(uint64_t& i, int b)
{
	uint64_t ig = (i >> (b + 1)) << b;
	uint64_t il = i & ((uint64_t(1) << b) - 1);
	i = ig + il;
	return i;
}

uchar rev_byte(uchar n)
{
	uchar rn = 0;
	for (int i = 7; i >= 0; --i)
		rn += get_bit(n, i) << (7 - i);
	return rn;
}

uchar inv_byte(uchar n)
{
	uchar rn = 0;
	for (int i = 7; i >= 0; --i)
		rn += !get_bit(n, i) << (7 - i);
	return rn;
}

void ham_cb_word(int* hw, int b)
{
	for (int i = b; i < H_W_LEN - 1; ++i)
		hw[i] = hw[i + 1];
	hw[H_W_LEN - 1] = 0;
}

void ham_dec_word(int* hw)
{
	uint64_t pos = 0;
	
	for (int i = 1; i <= H_W_LEN; i++)
		if (hw[i - 1])
			for (int j = 1; j <= 5; j++)
				if (get_bit(i, j - 1))
					inv_bit(pos, j - 1);
	
	
	//pos got
	if (pos > 0)
		hw[pos - 1] = !(hw[pos - 1]);
	//clearing
	ham_cb_word(hw, 15); 
	ham_cb_word(hw, 7);
	ham_cb_word(hw, 3);
	ham_cb_word(hw, 1);
	ham_cb_word(hw, 0);
}



string hamming_decode(bool oct_rev, bool word_rev, bool byte_rev) {
	int bitstr[BS_LEN];
	int decstr[DSTR_LEN];
	int bsi = 0;
	//transforming octs to bit_stream
	for (int osi = 0; osi < H_IN_SZ; ++osi)
		for (int oi = 0; oi < 8; ++oi)
			if (!oct_rev)
				bitstr[bsi++] = get_bit(H_IN[osi], oi);
			else
				bitstr[bsi++] = get_bit(H_IN[osi], 7 - oi);
	//padding
	for (; bsi < BS_LEN; ++bsi)
		bitstr[bsi] = 0;
	//extracting words
	int ham_word[H_W_LEN];
	for (int hwi = 0; hwi < W_NUM; ++hwi) {
		//copying word
		for (int bi = 0; bi < H_W_LEN; ++bi)
			if (!word_rev)
				ham_word[bi] = bitstr[hwi * H_W_LEN + bi];
			else
				ham_word[bi] = bitstr[hwi * H_W_LEN + (H_W_LEN - 1 - bi)];
		//fixing it
		ham_dec_word(ham_word);
		//copyback
		const bool wrev_back = !word_rev; //TODO: can fail here
		for (int bi = 0; bi < DW_LEN; ++bi)
			if (!wrev_back)
				decstr[hwi * DW_LEN + bi] = ham_word[bi];
			else
				decstr[hwi * DW_LEN + bi] = ham_word[DW_LEN - 1 - bi];
	}
	//padding, again
	for (int i = DEC_LEN; i < DSTR_LEN; ++i)
		decstr[i] = 0;
	//decoding
	string retstr;
	for (int ci = 0; ci < DSTR_BYTES; ++ci) {
		char c = '\0';
		for (int bi = 0; bi < 8; ++bi)
			if (!byte_rev)
				c += (decstr[ci * 8 + bi] << bi);
			else
				c += (decstr[ci * 8 + bi] << (7 - bi));
		retstr += c;
	}
	//cut
	while (retstr[retstr.length() - 1] == '\0')
		retstr.pop_back();
	return retstr;
}

void ham_ib_word(int* hw, int b, bool a) //insert bytes to word
{
	for (int i = H_W_LEN - 1; i > b; --i)
		hw[i] = hw[i - 1];
	hw[b] = a;
}

void ham_enc_word(int* hw)
{
	//adding control bits
	ham_ib_word(hw, 0, 0);
	ham_ib_word(hw, 1, 0);
	ham_ib_word(hw, 3, 0);
	ham_ib_word(hw, 7, 0);
	ham_ib_word(hw, 15, 0);
	//counting
	
	bool ctrl[5] = { 0,0,0,0,0 };

	for (int i = 1; i <= H_W_LEN; i++)
		if (hw[i - 1])
			for (int j = 1; j <= 5; j++)
				if (get_bit(i, j - 1))
					ctrl[j - 1] = !ctrl[j - 1];
	
	
	//writing
	hw[0] = ctrl[0];
	hw[1] = ctrl[1];
	hw[3] = ctrl[2];
	hw[7] = ctrl[3];
	hw[15] = ctrl[4];
}

int* hamming_encode(bool oct_rev, bool word_rev, bool byte_rev) {
	int respstr[RS_LEN];
	int encstr[ESTR_LEN];
	int rsi = 0;

	//transforming response bytes to bit_stream
	for (int ci = 0; ci < RET_SZ; ++ci)
		for (int bi = 0; bi < 8; ++bi)
			if (!byte_rev)
				respstr[rsi++] = get_bit(RET[ci], bi);
			else
				respstr[rsi++] = get_bit(RET[ci], 7 - bi);
	//padding
	for (; rsi < RS_LEN; ++rsi)
		respstr[rsi] = 0;

	const bool wrev_back = !word_rev; //TODO: can fail here

	//extracting words
	int ham_word[H_W_LEN];
	for (int hwi = 0; hwi < RW_NUM; ++hwi) {
		//copying word
		for (int bi = 0; bi < DW_LEN; ++bi)
			if (!wrev_back)
				ham_word[bi] = respstr[hwi * DW_LEN + bi];
			else
				ham_word[bi] = respstr[hwi * DW_LEN + (DW_LEN - 1 - bi)];
		//fixing it
		ham_enc_word(ham_word);
		//copyback
		for (int bi = 0; bi < H_W_LEN; ++bi)
			if (!word_rev)
				encstr[hwi * H_W_LEN + bi] = ham_word[bi];
			else
				encstr[hwi * H_W_LEN + bi] = ham_word[H_W_LEN - 1 - bi];
	}
	//padding, again
	for (int i = ENC_LEN; i < ESTR_LEN; ++i)
		encstr[i] = 0;

	//coding to octs
	int* octs = new int[ENC_BYTES];

	for (int oi = 0; oi < ENC_BYTES; ++oi) {
		octs[oi] = 0;
		for (int bi = 0; bi < 8; ++bi)
			if (!oct_rev)
				octs[oi] += (encstr[oi * 8 + bi] << bi);
			else
				octs[oi] += (encstr[oi * 8 + bi] << (7 - bi));
	}
	return octs;
}

/*
void hamming_decode(ifstream &ifs, ofstream &ofs)
{

	uint64_t t;
	uint64_t rd = 0, dec = 0;
	uchar c;
	int rd_bits = 0;
	int dec_bits = 0;
	ifs >> c;
	while (c != ']') {
		ifs >> t >> c;
		//for (int i = 0; i <= 7; i++) {//assuming that bits start from MSB
		for (int i = 7; i >= 0; i--) {//assuming that bits start from LSB
			set_bit(rd, rd_bits, get_bit(t, i));
			if (++rd_bits == 31) {//assuming that 31 WITH additional bytes
				//rdode
				dec += (hamming_fix(rd) << dec_bits);
				dec_bits += 26;
				while (dec_bits >= 8) {
					uchar dc = rev_byte(dec & 255); //last 8
					dec >>= 8;
					dec_bits -= 8;
					ofs << dc;
				}
				rd = 0;
				rd_bits = 0;
			}
		}
		//forget about last, it WILL be null))
	}
}
*/

unsigned int crc(int *bitstr, int STR_SIZE, bool inv)
{

	int POLY[33] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	POLY[32] = 1;
	POLY[30] = 1;
	POLY[29] = 1;
	POLY[28] = 1;
	POLY[26] = 1;
	POLY[20] = 1;
	POLY[19] = 1;
	POLY[17] = 1;
	POLY[16] = 1;
	POLY[15] = 1;
	POLY[11] = 1;
	POLY[10] = 1;
	POLY[7] = 1;
	POLY[6] = 1;
	POLY[4] = 1;
	POLY[2] = 1;
	POLY[1] = 1;
	POLY[0] = 1;

	//inverse first 32 BITS
	for (int i = 0; i < 32; ++i)
		bitstr[i] ^= 1;
	//prepare
	int SR[33];
	for (int sri = 32; sri >= 0; --sri)
		SR[sri] = 0;
	unsigned int FCS = 0;

	for (int bsi = 0; bsi < STR_SIZE; ++bsi) {
		//shift
		for (int sri = 32; sri > 0; --sri)
			SR[sri] = SR[sri - 1];
		SR[0] = bitstr[bsi];
		if (SR[32])
			for (int sri = 32; sri >= 0; --sri)
				SR[sri] = SR[sri] ^ POLY[sri];
	}

	//We're done, inverting to get FCS
	for (int sri = 31; sri >= 0; --sri)
		if (inv)
			FCS += (SR[sri] ^ 1) << sri;
		else
			FCS += SR[sri] << sri;
	return FCS;
}

string fcs()
{
	stringstream ss;

	const int PACK_SIZE = 64;
	const int BIT_SIZE = PACK_SIZE * 8;

	const int BIT_SIZE_FCS = PACK_SIZE * 8 + 32;

	uchar packet[PACK_SIZE];

	// DST address
	for (int pi = 0; pi < 6; ++pi)
		packet[pi] = 0xFF;

	// SRC address
	packet[6] = 0xBC;
	packet[7] = 0x1B;
	packet[8] = 0xB1;
	packet[9] = 0x0F;
	packet[10] = 0xB6;
	packet[11] = 0x51;

	//ARP
	packet[12] = 0x08;
	packet[13] = 0x06;

	//Ethernet
	packet[14] = 0x00;
	packet[15] = 0x01;

	//IPv4
	packet[16] = 0x08;
	packet[17] = 0x00;

	//Sizes
	packet[18] = 0x06;
	packet[19] = 0x04;

	//Opcode: ask
	packet[20] = 0x00;
	packet[21] = 0x01;

	// SRC MAC
	for (int pi = 22; pi < 28; ++pi)
		packet[pi] = packet[pi - 16];
	
	// SRC IP
	packet[28] = 215;
	packet[29] = 94;
	packet[30] = 40;
	packet[31] = 63;

	// DST address
	for (int pi = 32; pi < 38; ++pi)
		packet[pi] = 0x00;

	//target ip
	packet[38] = 21;
	packet[39] = 55;
	packet[40] = 49;
	packet[41] = 210;
	

	//padding
	for (int pi = 42; pi < PACK_SIZE; ++pi)
		packet[pi] = 0;

	int* bitstr = new int[BIT_SIZE];
	for (int pi = 0; pi < PACK_SIZE; ++pi)
		for (int bi = 0; bi < 8; ++bi)
			bitstr[pi * 8 + bi] = get_bit(packet[pi], bi);

	//calc CRC
	unsigned int fcs = crc(bitstr, BIT_SIZE);
	ss << "FCS = " << fcs << '\n';

	//clear
	delete[] bitstr;
	bitstr = new int[BIT_SIZE_FCS];

	//add fcs
	for (int ci = 0; ci < 4; ++ci)
		packet[PACK_SIZE - 4 + ci] = (fcs << 8 * ci) >> 8 * 3;
	
	//write again
	for (int pi = 0; pi < PACK_SIZE - 4; ++pi)
		for (int bi = 0; bi < 8; ++bi)
			bitstr[pi * 8 + bi] = get_bit(packet[pi], bi);

	for (int pi = PACK_SIZE - 4; pi < PACK_SIZE; ++pi)
		for (int bi = 0; bi < 8; ++bi)
			bitstr[pi * 8 + bi] = get_bit(packet[pi], 7 - bi);

	for (int bi = BIT_SIZE; bi < BIT_SIZE_FCS; ++bi)
		bitstr[bi] = 0;

	//magic is HERESY
	unsigned int magic_number = crc(bitstr, BIT_SIZE_FCS, 0);

	ss << "HERESY number = " << magic_number;

	return ss.str();
}

double cat()
{
	Cosmonet c;
	return c.calc_prob_good();
}
