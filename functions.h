#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
unsigned char S[256];
unsigned char INVP[256];
unsigned char P[256];
unsigned char SInv[256];
unsigned char round_key[44];
unsigned char Rcon[256];
unsigned char state[4][4];
unsigned char out_e[16];

#define xtime(x) ((x<<1) ^ (((x>>7) & 1) * 0x1b))

void modprod(char * poly1, char * poly2);
void modprodcheck(char * poly1, char * poly2);
unsigned char bigdot(unsigned char a, unsigned char b);
unsigned char cal_dec(unsigned char ch);

int checkHex(char * poly);
void tablecheck(char * filename);


void encrypt(char * tfilename, char * key , char *ipfilename);
void keyexpand(char *key, char *tablefile, int flag);

void decrypt(char * tfilename, char * key , char *ipfilename);
void inverse(char * p);

void compute_table_array(int array_type,char *equal_pos);
unsigned char Hex_to_Dec(char * array, int len, int start);

void AddRoundKey(int number);
