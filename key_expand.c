#include "functions.h"

unsigned char Hex_to_Dec(char * array, int len, int start) 
{
	unsigned char dec_value = 0;
	int i;

	// compute the value
	for (i=0; i<len; i++)
	{
		char c = array[start + i];

		// update value
		if ((c >= '0' && c <= '9')) 
		{
			dec_value = dec_value * 16 + c - '0';
		} 
		else if ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) 
		{
			dec_value = dec_value * 16 + c - 'a' + 10;
		} 

	}
	// return
	return dec_value;
}

void compute_table_array(int array_type,char *equal_pos)
{
	int i;
	char * temp = calloc(2, sizeof(char));
	if(array_type==1)
	{
		for(i=0;i<256;i++)
		{
			int j;
			strncpy(temp, equal_pos, 2);
			equal_pos+=2;
			unsigned char value;

			// compute the value
			for (j = 0; j < 2; j++)
			{
				char c = temp[j];
				value=cal_dec(c);
			}
			S[i] = value;
		}
	}
	else if(array_type==2 || array_type==3)
	{
		for(i=0;i<4;i++)
		{
			int j;
			strncpy(temp, equal_pos, 2);
			equal_pos+=2;
			// initialize value
			unsigned char value;

			// compute the value
			for (j=0;j<2;j++)
			{
				char c = temp[j];
				value=cal_dec(c);

			}
			if(array_type==2)
				P[i] = value;
			else if(array_type==3)
				INVP[i] = value;
		}
	}
}
void keyexpand(char *key, char *tablefile, int flag)
{
	//first check the integrity of tablefile
	char ch;
	int lines=0;
	int i;

	FILE *fp = fopen(tablefile, "rb");
	if(fp==NULL)
	{
		fprintf(stderr, "Error opening table file\n");
		exit(0);
	}
	while((ch = fgetc(fp)) != EOF)
	{
		if(ch=='\n'|| ch==10 || ch=='\0')
		{
			lines++;
		}
		else if(ch==' ' || ch=='\t' )
        {
        	fprintf(stderr, "Incorrect table file format\n" );
        	exit(0);
        }
	}
	if(lines!=3)
	{
		fprintf(stderr, "Incorrect table file format\n");
		exit(0);
	}
		//printf("%d\n", strlen(key));

	if(strlen(key)!=32)
	{
		fprintf(stderr, "Bad key, not enough characters\n");
		exit(0);
	}
	rewind(fp);
//========================================================
	int m;
	char *table = calloc(600, sizeof(char));
	table[0] = '\0';
	char *test = calloc(4, sizeof(char));
	test[0] = '\0';
	int array_type=0;//to check if S,P or invp(1-S;2-P;3-invp)
	for(m=0;m<3;m++) // read table file into arrays S, P and INVP
	{

		if(fgets(table,600, fp)==NULL)
			break;
		char* equal_pos = strchr(table,'=');
		
		int equal_pos_int = (int)(equal_pos-table);

		strncpy(test, table, equal_pos_int);
		test[equal_pos_int+1] = '\0';
		equal_pos++;
		
		if(strncmp(test, "S",1)==0 )
		{
			array_type=1;
			compute_table_array(array_type,equal_pos);


    	}
    	if(strncmp(test, "P", 2)==0 )
		{
			array_type=2;
			compute_table_array(array_type,equal_pos);
    		
    	}
    	if(strncmp(test, "INVP", 2)==0 )
		{
			array_type=3;
    		compute_table_array(array_type,equal_pos);
    	}

	}
/*********************Filled S, P, INVP arrays********************************/
	// initializing Rcon array. 
	Rcon[0] = 0x8d;
	for(i=1;i<255;i++) 
	{
		Rcon[i] = bigdot(Rcon[i - 1], 0x02);
	}

	unsigned char temp_key[4];
	int j;
	int temp;
	// first 4 keys remain same as the given key. 
	for(i=0;i<4;i++)
	{
		round_key[4*i] = Hex_to_Dec(key, 2, i*8);//length is 2, start is i*8 
		round_key[4*i+1] = Hex_to_Dec(key, 2, i*8+2);
		round_key[4*i+2] = Hex_to_Dec(key, 2, i*8+4);
		round_key[4*i+3] =  Hex_to_Dec(key, 2, i*8+6);
	}
	for(i=4;i<44;i++)
	{
		//printf("%dloop\n",i );
		for(j=0;j<4;j++) // putting the 4th set of 32 bits into temp_key from round_key i.e., 4 characters(32 bits) from round_key.
		{
			temp_key[j] = round_key[(i-1)*4 + j];
		}
		if(i%4 == 0)
		{
			// rotWord
			temp = temp_key[0];
			temp_key[0] = temp_key[1];
			temp_key[1] = temp_key[2];
			temp_key[2] = temp_key[3];
			temp_key[3] = temp;

			//SubstituteWord using S array. Since S is a 1D array with hex values converted to binary, subWord becomes a 1D array lookup. 
			temp_key[0] = S[temp_key[0]];
			temp_key[1] = S[temp_key[1]];
			temp_key[2] = S[temp_key[2]];
			temp_key[3] = S[temp_key[3]];
	
			temp_key[0] = temp_key[0] ^ Rcon[i/4];

		}
		// populating next 4 key bytes
		round_key[4*i]=round_key[4*(i-4)] ^ temp_key[0];
		round_key[4*i+1]=round_key[4*(i-4) + 1] ^ temp_key[1];
		round_key[4*i+2]=round_key[4*(i-4) + 2] ^ temp_key[2];
		round_key[4*i+3]=round_key[4*(i-4) + 3] ^ temp_key[3];

	}
	if(flag==1)
	{
		for (i=0;i<44;i++)
		{
			printf("w[%2d]: %02x%02x%02x%02x\n", i, round_key[4*i], round_key[4*i+1], round_key[4*i+2], round_key[4*i+3]);
		}	
	}
	
}
