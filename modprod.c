#include "functions.h"

unsigned char bigdot(unsigned char byte1, unsigned char byte2) // function to perform big dot notation. Computes a(bigdot)b
{

	unsigned char res = 0;
	unsigned char d = byte2;
	int i;
	for (i=0;i<8;i++) 
	{
		if (byte1%2 == 1)
			res= res^ d;
		byte1=byte1/2;
		d=xtime(d);
	}
   return res;
}


void modprod(char *poly1, char *poly2) // calculates and prints mod product given two polynomials. 
{

	unsigned char a[4], b[4];
	char * temp = calloc(2, sizeof(char));
	int i;

	if((!checkHex(poly1)) || (!checkHex(poly2)))	
	{
		fprintf(stderr, "The polynomials should be in hexadecimal format and should be of length 8.\n");
		exit(0);
	}
	else
	{

		//extracting poly1
		for(i=0;i<4;i++)
		{
			int j;
			unsigned char dec_value;
			strncpy(temp, poly1, 2);//each time two charcters will be taken(2 hex values) 
			//and their corresponding decimal equivalent will be found
			poly1+=2;
		
			// compute the value
			for (j = 0; j < 2; j++)
			{
				char ch = temp[j];
				dec_value=cal_dec(ch);
			}
			a[i] = dec_value;
		}

		//extracting poly2
		for(i=0;i<4;i++)
		{
			int j;
			strncpy(temp, poly2, 2);
			poly2+=2;
			unsigned char dec_value;

			// compute the value
			for (j = 0; j < 2; j++)
			{
				char ch = temp[j];
				dec_value=cal_dec(ch);
			}
			b[i] = dec_value;
		}
		unsigned char res[4]; 
		// matrix multiplication
		res[3] = bigdot(a[3], b[3]) ^ bigdot(a[0], b[2]) ^ bigdot(a[1], b[1]) ^ bigdot(a[2], b[0]);
		res[2] = bigdot(a[2], b[3]) ^ bigdot(a[3], b[2]) ^ bigdot(a[0], b[1]) ^ bigdot(a[1], b[0]);
		res[1] = bigdot(a[1], b[3]) ^ bigdot(a[2], b[2]) ^ bigdot(a[3], b[1]) ^ bigdot(a[0], b[0]);
		res[0] = bigdot(a[0], b[3]) ^ bigdot(a[1], b[2]) ^ bigdot(a[2], b[1]) ^ bigdot(a[3], b[0]);
		

		//printing
		printf("{%02x}{%02x}{%02x}{%02x} CIRCLEX {%02x}{%02x}{%02x}{%02x} = {%02x}{%02x}{%02x}{%02x}\n"
			, a[0], a[1], a[2], a[3], b[0], b[1], b[2], b[3], res[0], res[1], res[2], res[3]);	
		
	}
}

void modprodcheck(char * poly1, char * poly2) // checks if modprod gives required value. This does not print anything. 
{

	unsigned char a[4], b[4];
	char * temp = calloc(2, sizeof(char));
	int i;

	if((!checkHex(poly1)) || (!checkHex(poly2)))	
	{
		fprintf(stderr, "The polynomials should be in hexadecimal format and should be of length 8.\n");
		exit(0);
	}
	else
	{
		//extracting poly1
		for(i=0;i<4;i++)
		{
			int j;
			strncpy(temp, poly1, 2);
			poly1+=2;
			unsigned char dec_value;

			// compute the value
			for (j=0;j<2;j++)
			{
				char ch= temp[j];
				dec_value=cal_dec(ch); 

			}
			a[i] = dec_value;
		}

		//extracting poly2
		for(i=0;i<4;i++)
		{
			int j;
			strncpy(temp, poly2, 2);
			poly2+=2;
			unsigned char dec_value;

			// compute the value
			for (j = 0; j < 2; j++)
			{
				char ch = temp[j];
				dec_value=cal_dec(ch); 

			}
			b[i] = dec_value;
		}

		unsigned char res[4]; 
		// matrix multiplication
		res[3] = bigdot(a[3], b[3]) ^ bigdot(a[0], b[2]) ^ bigdot(a[1], b[1]) ^ bigdot(a[2], b[0]);
		res[2] = bigdot(a[2], b[3]) ^ bigdot(a[3], b[2]) ^ bigdot(a[0], b[1]) ^ bigdot(a[1], b[0]);
		res[1] = bigdot(a[1], b[3]) ^ bigdot(a[2], b[2]) ^ bigdot(a[3], b[1]) ^ bigdot(a[0], b[0]);
		res[0] = bigdot(a[0], b[3]) ^ bigdot(a[1], b[2]) ^ bigdot(a[2], b[1]) ^ bigdot(a[3], b[0]);


		if( (res[0]!=0) || (res[1]!=0) || (res[2]!=0) || (res[3]!=1) )
		{
			fprintf(stderr, "Product of the polynomials represented by P and INVP is not {00}{00}{00}{01}\n");
			exit(0);
		}
		
	}
}
