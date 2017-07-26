#include "functions.h"

/*referred from https://github.com/samyuktr/AES/blob/master/encrypt.c*/
void getSInv()
{
	int i;
	for(i=0;i<256;i++)
	{
		SInv[S[i]] = i;
	}

}

void invSubBytes() 
{
	int i, j;
	for (i=0;i<4;i++) 
	{
		for (j=0;j<4;j++) 
		{
			state[i][j] = SInv[state[i][j]];
		}
	}

}

void invShiftRows()
{
	unsigned char temp1; 
	unsigned char temp2;
	unsigned char temp;

	//single shift
	temp = state[1][3];
	state[1][3] = state[1][2];
	state[1][2] = state[1][1];
	state[1][1] = state[1][0];
	state[1][0] = temp;

	// double shift
	temp1 = state[2][3];
	temp2 = state[2][2];
	state[2][3] = state[2][1];
	state[2][2] = state[2][0];
	state[2][1] = temp1;
	state[2][0] = temp2;

	//triple shift
	temp = state[3][0];
	state[3][0] = state[3][1];
	state[3][1] = state[3][2];
	state[3][2] = state[3][3];
	state[3][3] = temp;
	//state[3][0] = temp;

}

void invMixColumns()
{

	unsigned char temp[4];
	int i;
	for (i=0;i<4;i++) 
	{		
		temp[0] = bigdot(INVP[3], state[0][i]) ^ bigdot(INVP[0], state[1][i]) ^ bigdot(INVP[1], state[2][i]) ^ bigdot(INVP[2], state[3][i]);
		temp[1] = bigdot(INVP[2], state[0][i]) ^ bigdot(INVP[3], state[1][i]) ^ bigdot(INVP[0], state[2][i]) ^ bigdot(INVP[1], state[3][i]);
		temp[2] = bigdot(INVP[1], state[0][i]) ^ bigdot(INVP[2], state[1][i]) ^ bigdot(INVP[3], state[2][i]) ^ bigdot(INVP[0], state[3][i]);
		temp[3] = bigdot(INVP[0], state[0][i]) ^ bigdot(INVP[1], state[1][i]) ^ bigdot(INVP[2], state[2][i]) ^ bigdot(INVP[3], state[3][i]);	


		state[0][i] = temp[0];
		state[1][i] = temp[1];
		state[2][i] = temp[2];
		state[3][i] = temp[3];

	}

}
void decrypt(char * tablefile, char * key , char *ipfile)
{
	int i, j;
	//printf("Hello!\n");
	char *input_str= calloc(18, sizeof(char));
	//	char * in_str;
	FILE *fp;
	if(ipfile!=NULL)
	{
		fp = fopen(ipfile, "rb");
	}
	else
	{
		 fp = stdin;
	}
	if(fp==NULL)
	{
		fprintf(stderr, "Error opening file\n");
		exit(0);
	}
	int input_count = 0;
	
	while (!feof(fp))
    {
        input_str[input_count++] = (unsigned char)getc(fp);
    }
	//printf("Count: %d \n", input_count);
	if((input_count-1)!=16)
	{
		fprintf(stderr, "Error in input file format.Less than 16bytes\n");
		exit(0);
	}

	//=====================Initial print statements========================================================

	printf("round[ 0].iinput   ");
	for(i=0;i<input_count-1;i++)
	{
		printf("%02x", (unsigned char)input_str[i]);
	}
	printf("\n");

	//printf("Key: %s\n", key);
	keyexpand(key, tablefile, 0);
	printf("round[ 0].ik_sch   ");
	for (i=0;i<4;i++) 
	{
		for (j=0;j<4;j++) 
		{
			printf("%02x", round_key[10*16 + 4*i+j]);
		}
	}
	printf("\n");

	//*********************************************************************************/
	int k= 0;
	// initialising state array with the cipher text. 
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			state[j][i] = input_str[k++];
		}
	}


	// getting inverse of the S table. 
	getSInv();
	/*********************************************************************************/
	AddRoundKey(10);

	int round;
	int count = 1;
	for(round=9;round>0;round--)
	{
		
		printf("round[%2d].istart   ", count);
		for (i=0;i<4;i++) 
		{
			for (j=0;j<4;j++) 
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");
	/*********************************************************************************/
		invShiftRows();

		printf("round[%2d].is_row   ", count);
		for (i=0;i<4;i++) 
		{
			for (j=0;j<4;j++) 
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");
		//==============================================================================
		invSubBytes();
		printf("round[%2d].is_box   ", count);
		for (i=0;i<4;i++) 
		{
			for (j=0;j<4;j++) 
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");

		//====================================================================================

		AddRoundKey(round);
		printf("round[%2d].ik_sch   ", count);
		for(i=0;i<4;i++) 
		{
			for(j=0;j<4;j++) 
			{
				printf("%02x", round_key[round*16 + 4*i+j] );
			}
		}
		printf("\n"); 
	/*********************************************************************************/
		printf("round[%2d].ik_add   ", count);
		for (i=0;i<4;i++) 
		{
			for (j=0;j<4;j++) 
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");
		//count++;
		//getchar();

		invMixColumns();
		count++;
	} // end of for loop
	/*********************************************************************************/

	printf("round[%2d].istart   ", count);
	for (i=0;i<4;i++) 
	{
		for (j=0;j<4;j++) 
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

	/*********************************************************************************/
	invShiftRows();

	printf("round[%2d].is_row   ", count);
	for (i=0;i<4;i++) 
	{
		for (j=0;j<4;j++) 
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");
	/*********************************************************************************/
	invSubBytes();
	printf("round[%2d].is_box   ", count);
	for (i=0;i<4;i++) 
	{
		for (j=0;j<4;j++) 
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

	/*********************************************************************************/
	
	printf("round[%2d].ik_sch   ", count);
	for(i=0;i<4;i++) 
	{
		for(j=0;j<4;j++) 
		{
			printf("%02x", round_key[round*16 + 4*i+j] );
		}
	}
	printf("\n"); 

	AddRoundKey(0);

	/*********************************************************************************/

	printf("round[%2d].ioutput  ", count);
	for (i=0;i<4;i++) 
	{
		for (j=0;j<4;j++) 
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

} 
