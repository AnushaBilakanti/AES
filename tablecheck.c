#include "functions.h"
//checks the validity of tablecheck
unsigned char Scheck[256];

int checkHex(char * poly) // checks if input is in hex format. 
{
	int i;
	int flag = 0;
	for(i=0;i<strlen(poly);i++)
	{
		char c = poly[i];
		if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
			flag = 1;
		else
			break;
	}
	return flag;
}
unsigned char cal_dec(unsigned char ch)
{
	unsigned char dec_value;
	if(ch >= '0' && ch <= '9') 
	{
		dec_value = dec_value * 16 + ch - '0';
	}
	else if ((ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))
	{
		dec_value = dec_value * 16 + ch - 'a' + 10;
	}
	return dec_value;
}
int testS(char * line) // tests S table
{
	if(!checkHex(line))
	{
		fprintf(stderr, "Error in S table. Values not in hex format\n");
		exit(0);
	}
	int m;
	
	char * temp = calloc(2, sizeof(char));
	for(m=0;m<256;m++)
	{
		int j;
		unsigned char dec_value;
		strncpy(temp, line, 2);
		line+=2;
		

		// compute the value
		for (j = 0; j < 2; j++)
		{
			char ch = temp[j];
			dec_value=cal_dec(ch);
		}

		if(Scheck[dec_value]==1)
		{
			fprintf(stderr, "Repeated entries in S.\n");
			exit(0);
		}
		else
		{
			Scheck[dec_value] = 1;
		}
	}
	return 1;
}

int testP(char * line) // tests P input
{
	if(strlen(line)-1!=8)
	{
		fprintf(stderr, "P table does not contain value with length 8\n");
		exit(0);
	}
	if(!checkHex(line))
	{
		fprintf(stderr, "Error in P table.Values not in hex format\n");
		exit(0);
	}
	
	return 1;
}

int testINVP(char * line) // tests inverse P input
{

	if(strlen(line)-1!=8)
	{
		fprintf(stderr, "INVP table does not contain value with length 8\n");
		exit(0);
	}

	if(!checkHex(line))
	{
		fprintf(stderr, "Error in INVP table.Values not in hex format\n");
		exit(0);
	}
	return 1;
}


void tablecheck(char * tablefile) // does necessary table check. 
{
	int flag_p=0, flag_ip=0;
	char ch;
	int lines = 0;
	int i;

	FILE * fp = fopen(tablefile, "rb");
	if(fp==NULL)
	{
		fprintf(stderr, "cannot open table file\n");
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
        	fprintf(stderr, "malformed table file\n" );
        	exit(0);
        }
	}
	if(lines!=3)
	{
		fprintf(stderr, "Incorrect table file format. S, P and INVP don't exist in the table.\n");
		exit(0);
	}
	rewind(fp);


	//========================================================

	char * table = calloc(600, sizeof(char));
	table[0] = '\0';
	char *test = calloc(4, sizeof(char));
	test[0] = '\0';
	char * poly1= calloc(8, sizeof(char));
	char * poly2 = calloc(8, sizeof(char));
	for(i=0;i<3;i++)
	{
		
		if(fgets(table, 600, fp)==NULL)
			break;
		char* equal_pos = strchr(table,'=');
		
		int equal_pos_int = (int)(equal_pos-table);

		strncpy(test, table, equal_pos_int);
		test[equal_pos_int+1] = '\0';
		equal_pos++;
		
		
		if(strncmp(test,"S",1)==0 )
		{
    		if(!testS(equal_pos))
    		{
    			fprintf(stderr, "S table error!\n");

    			exit(0);
    		}
    	}
    	else if(strncmp(test, "P", 1)==0)
    	{
    			
			if(!testP(equal_pos))
			{
				fprintf(stderr, "P table error!\n");
				exit(0);
			}
			strncpy(poly1, equal_pos, 8);
			flag_p = 1;
    	}
    	else if(strncmp(test, "INVP", 4)==0)
    	{
    			
			if(!testINVP(equal_pos))
			{
				fprintf(stderr, "INVP table error!\n");
				exit(0);
			}
			strncpy(poly2, equal_pos, 8);
			flag_ip = 1;
    	}
    }
    if(flag_p==0)
    {
    	fprintf(stderr, "P table does not exist :\n");
    	exit(0);
    }
    if(flag_ip==0)
    {
    	fprintf(stderr, "INVP table does not exist :\n");
    	exit(0);
    }
    modprodcheck(poly1, poly2);
}
