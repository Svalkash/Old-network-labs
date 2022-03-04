#pragma once

#include <fstream>
#include <string>
#include <cmath>
#include "cosmonet.h"




const int H_IN_SZ = 35;
const int H_IN[] = { 138, 169, 86, 38, 143, 147, 32, 157, 201, 200, 228, 24, 231, 99, 184, 3, 80, 24, 92, 88, 153, 12, 35, 76, 172, 142, 52, 222, 2, 7, 1, 16, 63, 129, 64 };
const int H_W_LEN = 31;

const int DW_LEN = H_W_LEN - 5; //TODO: replace 5 by ceil


const int W_NUM = (H_IN_SZ * 8 / H_W_LEN) + (H_IN_SZ * 8 % H_W_LEN > 0);
const int BS_LEN = W_NUM * H_W_LEN;
// ceil(log2(H_W_LEN))
const int DEC_LEN = DW_LEN * W_NUM;
const int DSTR_BYTES = (DEC_LEN / 8) + (DEC_LEN % 8 > 0);
const int DSTR_LEN = DSTR_BYTES * 8;



/*
const int H_IN_SZ = 20;
const int H_IN[] = { 51, 18, 144, 215, 147, 134, 199, 86, 204, 199, 102, 103, 98, 96, 177, 0, 0, 0, 156, 64 };
const int H_W_LEN = 31;

const int DW_LEN = H_W_LEN - 5; //TODO: replace 5 by ceil


const int W_NUM = (H_IN_SZ * 8 / H_W_LEN) + (H_IN_SZ * 8 % H_W_LEN > 0);
const int BS_LEN = W_NUM * H_W_LEN;
// ceil(log2(H_W_LEN))
const int DEC_LEN = DW_LEN * W_NUM;
const int DSTR_BYTES = (DEC_LEN / 8) + (DEC_LEN % 8 > 0);
const int DSTR_LEN = DSTR_BYTES * 8;
*/


const int RET_SZ = 15;
const char RET[] = "12364996354272";

const int RW_NUM = (RET_SZ * 8 / DW_LEN) + (RET_SZ * 8 % DW_LEN > 0);
const int RS_LEN = RW_NUM * DW_LEN;
// ceil(log2(H_W_LEN))
const int ENC_LEN = H_W_LEN * RW_NUM;
const int ENC_BYTES = (ENC_LEN / 8) + (ENC_LEN % 8 > 0);
const int ESTR_LEN = ENC_BYTES * 8;




using namespace std;

typedef unsigned char uchar;

//almost UNUSED funcs to get bits from numbers
bool get_bit(uint64_t i, int b);
uint64_t &set_bit(uint64_t& i, int b, bool t);
uint64_t &inv_bit(uint64_t& i, int b);
uint64_t &add_bit(uint64_t& i, int b);

uint64_t& clear_bit(uint64_t& i, int b);

uchar rev_byte(uchar n);

uchar inv_byte(uchar n);

//remove bit from word and shift others
void ham_cb_word(int* hw, int b);
//decode word
void ham_dec_word(int*hw);
//decode function, gets control direction bits
string hamming_decode(bool oct_rev, bool word_rev, bool byte_rev);
//insert bit
void ham_ib_word(int* hw, int b, bool a);
//encode word
void ham_enc_word(int* hw);
//what does it mean...?
int *hamming_encode(bool oct_rev, bool word_rev, bool byte_rev);

unsigned int crc(int *bitstr, int STR_SIZE, bool inv = 1);
string fcs();
//string fcs_1();

double cat();