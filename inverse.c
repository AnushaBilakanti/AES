#include "functions.h"

/*referred from https://github.com/kevincrane/cryptography-projects/blob/master/hw6-aes/inverse.c*/


unsigned int getinv(int index) 
{

	char* table_list; 
	table_list = (char*)"00018df6cb527bd1e84f29c0b0e1e5c7\
74b4aa4b992b605f583ffdccff40eeb2\
3a6e5af1554da8c9c10a98153044a2c2\
2c45926cf3396642f235206f77bb5919\
1dfe37672d31f569a764ab135425e909\
ed5c05ca4c2487bf183e22f051ec6117\
165eafd349a63643f44791df3393213b\
79b7978510b5ba3cb670d006a1fa8182\
837e7f809673be569b9e95d9f702b9a4\
de6a326dd88a84722a149f88f9dc899a\
fb7c2ec38fb8654826c8124acee7d262\
0ce01fef11757871a58e763dbdbc8657\
0b282fa3dad4e40fa92753041bfcace6\
7a07ae63c5dbe2ea948bc4d59df8906b\
b10dd6ebc60ecfad084ed7e35d501eb3\
5b2338346846038cdd9c7da0cd1a411c";

	// Read each pair of hexvals, store as ASCII vals in inv_table
	char temp_val[3];
	temp_val[0] = table_list[index * 2];
	temp_val[1] = table_list[index * 2 + 1];
	temp_val[2] = ' ';

	return strtol(temp_val, NULL, 16);
}
void inverse(char * poly)
{
	if(strlen(poly)!=8)
	{
		fprintf(stderr, "Input polynomial not in right format. Need to have 8 characters.\n");
		exit(0);
	}
	int i;
	int flag;
	for(i=0;i<strlen(poly);i++)
	{
		char c = poly[i];
		if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
			flag = 1;
		else
		{
			fprintf(stderr, "Invalid polynomial\n");
			exit(0);
		}
	}
	
		
	char * temp = calloc(2, sizeof(char));
	unsigned char poly_num[4];
	int  j;
	for(i=0;i<4;i++)
	{
		int j;
		unsigned char value;
		strncpy(temp, poly, 2);
		poly+=2;

		// compute the value
		for (j=0; j<2; j++)
		{
			char c = temp[j];
			value=cal_dec(c); 
		}
		poly_num[i] = value;

	}
	//Initially we know 2 rows of rem, and 2 rows of aux. 
	unsigned char rem[6][5] = { { 0x01, 0x00, 0x00, 0x00, 0x01 }, { 0x00, poly_num[0], poly_num[1], poly_num[2], poly_num[3] } };
	unsigned char quo[6][4] = { { 0 } };
	unsigned char aux[6][4] = { { 0 }, { rem[0][1], rem[0][2], rem[0][3], rem[0][4] } };
	//printing what we know in the initial 2 rows. 

	printf("i=1, rem[i]={%02x}{%02x}{%02x}{%02x}, quo[i]={00}{00}{00}{00}, aux[i]={00}{00}{00}{00}\n", rem[0][1], rem[0][2], rem[0][3], rem[0][4]);
	printf("i=2, rem[i]={%02x}{%02x}{%02x}{%02x}, quo[i]={00}{00}{00}{00}, aux[i]={%02x}{%02x}{%02x}{%02x}\n", poly_num[0], poly_num[1], poly_num[2], poly_num[3], aux[1][0], aux[1][1], aux[1][2],	aux[1][3]);

	for (i=2;i<6;i++) // populating the table from 2 to 6. 
	{
		// first division i.e., with one value in quotient quo[0]
		unsigned char temp[6 - i];
		quo[i][2] = bigdot(getinv(rem[i-1][i-1]), rem[i-2][i-2]); // getting the inverse of the first coeff when i=2. 
		for (j=0;j<(6 - i);j++) {
			temp[j] = rem[i-2][j+i-1] ^ bigdot(rem[i-1][j+i], quo[i][2]); // bigdot with inverse and xor with what is in the dividend's place at position j
		}

		// Till the 5th row we do the next steps. 
		if (i<5) 
		{
			temp[5-i] = rem[i-2][4];
			quo[i][3] = bigdot(getinv(rem[i-1][i-1]), temp[0]); // getting a value that will give {01} while temp[0] has what was calculated in the first bigdot operation
		} 
		else // if its the 5th row, check if it multiplies to 0x01
		{
			quo[i][3] = bigdot(getinv(rem[i-1][i-1]), temp[0] ^ 0x01); 
			rem[i][i-1] = 0x01;
		}
		// Compute remainder from quotient and previous remainders
		for (j=i;j<5;j++) 
		{
			rem[i][j] = temp[j-i+1] ^ bigdot(rem[i-1][j], quo[i][3]);
		}
		//Now we have all quo values. For aux, we need to get the mod product of value at position i in quo and i-1 in aux for every coeff and xor them 

		aux[i][3] = bigdot(quo[i][3], aux[i - 1][3]) ^ bigdot(quo[i][0], aux[i - 1][2]) ^ bigdot(quo[i][1], aux[i - 1][1]) ^ bigdot(quo[i][2], aux[i - 1][0]);
		aux[i][2] = bigdot(quo[i][2], aux[i - 1][3]) ^ bigdot(quo[i][3], aux[i - 1][2]) ^ bigdot(quo[i][0], aux[i - 1][1]) ^ bigdot(quo[i][1], aux[i - 1][0]);
		aux[i][1] = bigdot(quo[i][1], aux[i - 1][3]) ^ bigdot(quo[i][2], aux[i - 1][2]) ^ bigdot(quo[i][3], aux[i - 1][1]) ^ bigdot(quo[i][0], aux[i - 1][0]);
		aux[i][0] = bigdot(quo[i][0], aux[i - 1][3]) ^ bigdot(quo[i][1], aux[i - 1][2]) ^ bigdot(quo[i][2], aux[i - 1][1]) ^ bigdot(quo[i][3], aux[i - 1][0]);

		for (j=0;j<4;j++) 
		{
			aux[i][j] ^= aux[i-2][j];
		}
		// Output current results
		printf("i=%d, rem[i]={%02x}{%02x}{%02x}{%02x}",i + 1, rem[i][1], rem[i][2], rem[i][3], rem[i][4]);
		printf(", quo[i]={%02x}{%02x}{%02x}{%02x}", quo[i][0], quo[i][1], quo[i][2], quo[i][3]);
		printf(", aux[i]={%02x}{%02x}{%02x}{%02x}\n", aux[i][0], aux[i][1], aux[i][2], aux[i][3]);

		// Invalid coefficient, no inverse
		if (i != 5 && rem[i][i] == 0x00) {
			printf("{%02x}{%02x}{%02x}{%02x} does not have a multiplicative inverse.\n",
					poly_num[0], poly_num[1], poly_num[2], poly_num[3]);
			
		}
	}

	printf("Multiplicative inverse of {%02x}{%02x}{%02x}{%02x} is {%02x}{%02x}{%02x}{%02x}\n",
			poly_num[0], poly_num[1], poly_num[2], poly_num[3], aux[5][0], aux[5][1],
			aux[5][2], aux[5][3]);	
}
