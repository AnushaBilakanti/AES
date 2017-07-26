#include "functions.h"
/*referred from https://github.com/samyuktr/AES/blob/master/encrypt.c*/
void AddRoundKey(int number)
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			state[j][i] = state[j][i]^round_key[(number*4*4) + (4*i)+j]; 
		}
	}
}

void SubBytes() 
{
	int i, j;
	for(i=0;i<4;i++) 
	{
		for(j=0;j<4;j++) 
		{
			state[i][j] = S[state[i][j]];
		}
	}
}


void ShiftRows()
{
	unsigned char temp1; 
	unsigned char temp2;
	unsigned char temp;

	//single shift
	temp = state[1][0];
	state[1][0] = state[1][1];
	state[1][1] = state[1][2];
	state[1][2] = state[1][3];
	state[1][3] = temp;

	// double shift
	temp1 = state[2][0];
	temp2 = state[2][1];
	state[2][0] = state[2][2];
	state[2][1] = state[2][3];
	state[2][2] = temp1;
	state[2][3] = temp2;

	//triple shift
	temp = state[3][0];
	state[3][0] = state[3][3];
	state[3][3] = state[3][2];
	state[3][2] = state[3][1];
	state[3][1] = temp;
}

void MixColumns()
{

	unsigned char *temp=calloc(2, sizeof(char));
	int i;
	for (i=0;i<4;i++) 
	{		
		temp[0] = bigdot(P[3], state[0][i]) ^ bigdot(P[0], state[1][i]) ^ bigdot(P[1], state[2][i]) ^ bigdot(P[2], state[3][i]);
		temp[1] = bigdot(P[2], state[0][i]) ^ bigdot(P[3], state[1][i]) ^ bigdot(P[0], state[2][i]) ^ bigdot(P[1], state[3][i]);
		temp[2] = bigdot(P[1], state[0][i]) ^ bigdot(P[2], state[1][i]) ^ bigdot(P[3], state[2][i]) ^ bigdot(P[0], state[3][i]);
		temp[3] = bigdot(P[0], state[0][i]) ^ bigdot(P[1], state[1][i]) ^ bigdot(P[2], state[2][i]) ^ bigdot(P[3], state[3][i]);	

		state[0][i] = temp[0];
		state[1][i] = temp[1];
		state[2][i] = temp[2];
		state[3][i] = temp[3];
	}

}

void encrypt(char *tablefile, char *key , char *ipfile)
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


	/***************printing initial statements**********************/
		printf("round[ 0].input    ");
		for(i=0;i<input_count-1;i++)
		{
			printf("%02x", (unsigned char)input_str[i]);
		}
		printf("\n");
		printf("round[ 0].k_sch    ");
		for(i=0;i<strlen(key);i++)
		{
			printf("%c", key[i] );
		}
		printf("\n");

	/****************copying input string to state array in a vertical manner******/
		int k= 0;
		for(i=0;i<4;i++)
		{
			for (j=0;j<4;j++) 
			{
				state[j][i] = input_str[k++];
			}
		}
	/************initalise round_key global variable**************/
		keyexpand(key,tablefile, 0);
		AddRoundKey(0); // initial round key operation. 
		int count = 1; // counter to print 
		int round;

		for(round=1;round<10;round++)
		{
			printf("round[%2d].start    ", count);
			for (i=0;i<4;i++) 
			{
				for (j=0;j<4;j++) 
				{
					printf("%02x", state[j][i]);
				}
			}
			printf("\n");
	/*********************************************************************************/
			SubBytes();
			printf("round[%2d].s_box    ", count);
			for (i=0;i<4;i++) 
			{
				for (j=0;j<4;j++) 
				{
					printf("%02x", state[j][i]);
				}
			}
			printf("\n");


	/*********************************************************************************/

			ShiftRows();

			printf("round[%2d].s_row    ", count);
			for (i=0;i<4;i++) 
			{
				for (j=0;j<4;j++) 
				{
					printf("%02x", state[j][i]);
				}
			}
			printf("\n");
	/*********************************************************************************/
			MixColumns();
			printf("round[%2d].m_col    ", count);
			for (i=0;i<4;i++) 
			{
				for (j=0;j<4;j++) 
				{
					printf("%02x", state[j][i]);
				}
			}
			printf("\n");
			
	/*********************************************************************************/
			AddRoundKey(round);
			printf("round[%2d].k_sch    ", count);
			for(i=0;i<4;i++) 
			{
				for(j=0;j<4;j++) 
				{
					printf("%02x", round_key[round*16 + 4*i+j] );
				}
			}
			printf("\n"); 
			count++;
	/*********************************************************************************/



		} // end of for loop. 

		printf("round[%2d].start    ", count);
		for (i=0;i<4;i++) 
		{
			for (j=0;j<4;j++) 
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");
	/*********************************************************************************/
		SubBytes();
		printf("round[%2d].s_box    ", count);
		for (i=0;i<4;i++) 
		{
			for (j=0;j<4;j++) 
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");
	/*********************************************************************************/
		ShiftRows();

		printf("round[%2d].s_row    ", count);
		for (i=0;i<4;i++) 
		{
			for (j=0;j<4;j++) 
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");
	/*********************************************************************************/
		AddRoundKey(10);
		printf("round[%2d].k_sch    ", count);
		for(i=0;i<4;i++) 
		{
			for(j=0;j<4;j++) 
			{
				printf("%02x", round_key[round*16 + 4*i+j] );
			}
		}
		printf("\n");
	/*********************************************************************************/
		int count_out_e = 0;
		//printf("round[%2d].output   ", count);
		for(i=0;i<4;i++) 
		{
			for(j=0;j<4;j++) 
			{
				out_e[count_out_e++] = state[j][i];
			}
		}

		printf("round[%2d].output   ", count);
		for(i=0;i<count_out_e;i++)
		{
			printf("%02x",out_e[i]);
		}
		printf("\n");
}
