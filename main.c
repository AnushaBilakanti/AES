#include "functions.h"

int main(int argc, char * argv[])
{
	char * poly1;
	char * poly2;
	char * key;
   	char * input_str;
	if(argc==1)
    {
        fprintf(stderr, "Error: Malformed command.\n");
        exit(0);
    }
    else
    {
        if(strcmp(argv[1], "tablecheck")==0)
        {	
        	if(argc!=3)
        	{
        		fprintf(stderr, "Error: Malformed command");
                exit(0);
        	}
        	else
        	{
 
        		if(argv[2][1]=='t')
                {
                    input_str = strchr(argv[2], '=');
                  
                    input_str++;
                }
             
                tablecheck(input_str);

        	}
        }

        if(strcmp(argv[1], "modprod")==0)
        {   
            if(argc!=4)
            {
                fprintf(stderr, "Error: Malformed command. Please enter in the following format \nhw5 modprod -p1=poly1 -p2=poly2\n");
                exit(0);
            }
            else
            {
 
                if((argv[2][1]=='p') && (argv[2][2]=='1'))
                {
                    poly1 = strchr(argv[2], '=');                  
                    poly1++;
                }

                if((argv[3][1]=='p') && (argv[3][2]=='2'))
                {
                    poly2 = strchr(argv[3], '=');                  
                    poly2++;
                }
                if(strlen(poly1)!=8)
                {
                    fprintf(stderr, "-p1 parameter be of length 8\n");
                    exit(0);
                }
                if(strlen(poly2)!=8)
                {
                    fprintf(stderr, "-p2 parameter be of length 8\n");
                    exit(0);
                }

                modprod(poly1, poly2);

            }
        }
        if(strcmp(argv[1], "keyexpand")==0)
        {
        	if(argc!=4)
        	{
        		fprintf(stderr, "Error: Malformed command. Please enter in the following format \nhw5 keyexpand -k=key -t=tablefile\n");
                exit(0);
        	}
        	else
        	{
        		if(argv[2][1]=='k')
                {
                    key = strchr(argv[2], '=');                  
                    key++;
                }

                if(argv[3][1]=='t')
                {
                    input_str = strchr(argv[3], '=');
                  
                    input_str++;
                }
                keyexpand(key, input_str, 1);
        	}
        }
        else if(strcmp(argv[1], "encrypt")==0)
        {
        	if(argc<4)
        	{
        		fprintf(stderr, "Error: Malformed command. Please enter in the following format \nhw5 encrypt -k=key -t=tablefile [file]\n");
                exit(0);	
        	}
        	if(argc==4)
        	{
        		if(argv[2][1]=='k')
        		{
        			key = strchr(argv[2], '=');
        			key++;
                    int i; 
                    int len=strlen(key);
                    if(len!=32)
                    {
                        fprintf(stderr, "Key length error!\n");
                        exit(0);
                    }
                    for(i=0;i<len;i++)
                    {
                        if(!( ( (key[i]>='a') && (key[i]<='f') ) || ( (key[i]>='0') && (key[i]<='9') )  ))
                        {
                            fprintf(stderr, "Invalid  key format.\n");
                            exit(0);;
                        }  
                       
                    }    
        		}
        		if(argv[3][1]=='t')
                {
                    input_str = strchr(argv[3], '=');
                    input_str++;
                }
                tablecheck(input_str);
                
                encrypt(input_str,key, NULL);
        	}
        	else if(argc==5)
        	{
        		if(argv[2][1]=='k')
        		{
                    
        			key = strchr(argv[2], '=');
        			key++;
                    int i; 
                    int len=strlen(key);
                    if(len!=32)
                    {
                        fprintf(stderr, "Key error!\n");
                        exit(0);
                    }
                    for(i=0;i<len;i++)
                    {
                        if(!( ( (key[i]>='a') && (key[i]<='f') ) || ( (key[i]>='0') && (key[i]<='9') )  ))
                        {
                            fprintf(stderr, "Invalid key.\n");
                            exit(0);;
                        }  
                       
                    }    
        		}
        		if(argv[3][1]=='t')
                {
                    input_str = strchr(argv[3], '=');
                    input_str++;
                }
                tablecheck(input_str);
                encrypt(input_str,key, argv[4]);
        	}
        }

        else if(strcmp(argv[1], "decrypt")==0)
        {
        	if(argc<4)
        	{
        		fprintf(stderr, "Error: Malformed command. Please enter in the following format \nhw5 decrypt -k=key -t=tablefile [file]\n");
                exit(0);	
        	}
        	if(argc==4)
        	{
        		if(argv[2][1]=='k')
        		{
        			key = strchr(argv[2], '=');
        			key++;
                    int i; 
                    int len=strlen(key);
                    if(len!=32)
                    {
                        fprintf(stderr, "Key length error!\n");
                        exit(0);
                    }
                    for(i=0;i<len;i++)
                    {
                        if(!( ( (key[i]>='a') && (key[i]<='f') ) || ( (key[i]>='0') && (key[i]<='9') )  ))
                        {
                            fprintf(stderr, "Invalid  key format.\n");
                            exit(0);;
                        }  
                       
                    }    
        		}
        		if(argv[3][1]=='t')
                {
                    input_str = strchr(argv[3], '=');
                    input_str++;
                }
                tablecheck(input_str);
                
                decrypt(input_str,key, NULL);
        	}
        	else if(argc==5)
        	{
        		if(argv[2][1]=='k')
        		{
                    
        			key = strchr(argv[2], '=');
        			key++;
                    int i; 
                    int len=strlen(key);
                    if(len!=32)
                    {
                        fprintf(stderr, "Key error!\n");
                        exit(0);
                    }
                    for(i=0;i<len;i++)
                    {
                        if(!( ( (key[i]>='a') && (key[i]<='f') ) || ( (key[i]>='0') && (key[i]<='9') )  ))
                        {
                            fprintf(stderr, "Invalid key.\n");
                            exit(0);;
                        }  
                       
                    }    
        		}
        		if(argv[3][1]=='t')
                {
                    input_str = strchr(argv[3], '=');
                    input_str++;
                }
                tablecheck(input_str);
                decrypt(input_str,key, argv[4]);
        	}
        }

        else if(strcmp(argv[1], "inverse")==0)
        {
        	if(argc!=3)
        	{
        		fprintf(stderr, "Error: Malformed command. Please enter in the following format \nhw5 inverse -p=poly\n");
                exit(0);
        	}
        	if(argv[2][1]=='p')
            {
                input_str = strchr(argv[2], '=');                  
                input_str++;
            }
            inverse(input_str);
        }
    }
	return 0;
}
