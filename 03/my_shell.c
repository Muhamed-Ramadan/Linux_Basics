#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


/* int execve(const char *filename, char *const argv[],
                  char *const envp[]); */

#define VALUE_SIZE     100
#define NAME_SIZE      50
#define NUMBER_OF_VAR  100
#define ARGV_SIZE      10
#define ARGV_STR_SIZE  25
int isVar(char* buf,int len);


int main()
{
	char buf[100]="0";
	char name[NUMBER_OF_VAR][NAME_SIZE];
	char value[NUMBER_OF_VAR][VALUE_SIZE];
	int var_index=0;
	int len;
	int return_value=0;
	int i;
	char* argv[ARGV_SIZE];
	//char* argv[ARGV_SIZE];
	char argc=1;
	char temp_buf[100];
	while(1)
	{
		return_value=0;
		argc=1;
		for (i=0;i<ARGV_SIZE;i++)
		{
			argv[i]=0;
		}
								
		
		
		printf("my Promp:");			// printing the user,current adress <<pending>>
		
		// taking input from user, and neglecting the '\n' character
		//scanf("%s",buf);
		fgets(buf,100,stdin);

		len = strlen(buf);
		if (buf[len-1]=='\n' || buf[len-1]==10 ) buf[len-1]=0;
		fflush(stdin);
		
		
		//removing spaces before the start of the command
		for (i=0;buf[i]==' ';i++);
		strcpy(buf,&buf[i]);
		

		//removing extra interior spaces	
		len = strlen(buf);	
		for (i=1;i<len;i++)
		{
			if (buf[i+1]==' ' && buf[i]==' ')
			{
				for(int j=i;j<len;j++)
				{
					buf[j]=buf[j+1];
				}
				len--;
				i--;
			}
		}
		if (buf[len-1]==' ') // removing space ch the the end of buf
		{
			buf[len-1]=0;
			len--;
		}
	
		// checking if the user pressed enter with no input 		<<done>>
		if (buf[0]==0)
			continue;
		
		len = strlen(buf);
		if (buf[len-1]=='\n' || buf[len-1]==10 ) buf[len-1]==0;
	
		//cheking if the argument is a variable (searching for "=")   <<done>>
		len = strlen(buf);
		return_value = isVar(buf,len);
		if (return_value>0)
		{
			strncpy(temp_buf,buf,return_value);
			for (i=0;i<var_index;i++)  // searching if the var is existed if so, modify its value
			{
				if (strcmp(name[i],temp_buf)==0 )
				{
					strcpy(value[i],&buf[return_value+1]); 
					// ++if the vairable is existed in env, we must modify the env <<pending>>
					//printf("var is existed\n");
					break;
				}
			}
			if (i==var_index) // for loop reached the limit, (variable was not found) = store itaa
			
			{
			
				strncpy(name[var_index],buf,return_value);
				strcpy(value[var_index],&buf[return_value+1]);
				var_index++;
				//printf("name=%s var=%s i=%d\n",name[var_index-1],value[var_index-1],var_index);
			}
			continue;
		}
		
		// checking for built in commands      //<<done>>
		if (strcmp("set",buf)==0 )             //set command  <<done>>
		{
			for (i=0;i<var_index;i++)
			{
				printf("%s = %s\n",name[i],value[i]);
			}
			continue;
		}
		else if (strncmp("export ",buf,7)==0 ) //export command   <<done>>
		{
			//printf("the word %s\n",&buf[7]);
			for (i=0;i<var_index;i++)  // searching for the variable name 
			{
				if (strcmp(name[i],&buf[7])==0 ) // 7 is the index of the string after "export "
				{
					return_value = setenv(name[i],value[i],1);	
					if (return_value<0)
					{
						printf("some thing wrong with exporting this variable\n");
					}
					break;
				}
			}
			if (i==var_index) // for loop reached the limit, variable was not found
				printf("no such variable\n");
			continue;
		}
		else if (strcmp("export",buf)==0 )  // export without a variable will do nothing
		{
			continue;
		}
		else if (strcmp("exit",buf)==0 )   // exiting the current shell
		{
			exit(0);
		}
		else if (strcmp("shell",buf)==0 )  // creating a child shell
		{
			return_value = fork();
			if (return_value>0)
			{
				wait(&return_value);
			}
			else if (return_value==0)
			{
				// continue to the head of the super loop
			}
			else if (return_value<0)
			{
				printf("fork failed\n");
			}
			continue;
			
		}
		// collecting argv and argc   <<done>>
		len=strlen(buf);
		argv[0]=buf;
		for (i=0;i<len;i++)
		{
			if (buf[i]==' ')
			{
				buf[i]=0;
				argc++;
				argv[argc-1]=&buf[i+1];
				// optimize i ( i = strlen[argv[argc-1] )
				
			}
		}
		
		// fork and execv works for other commands  <<done>>
		return_value=fork();
		if (return_value<0)
		{
			printf("fork failed\n");
			continue;
		}
		else if (return_value>0)
		{
			wait(&return_value);
			continue;
		}
		
		else if (return_value==0)
		{
			if (strcmp(argv[0],"ls")==0 ) // color=auto
			{
				char arr[]="--color=auto";
				argv[argc++]=arr;
			}
			execvp(argv[0], argv);
			printf("wrong command\n");
			return -1;
		}
		
		
			
	}
	return 0;
}


int isVar(char* buf,int len) // return values: -1: if first element is'=', -2: , 
//-3: if first element is number , -4: no '=' was found, -5: wrong name
{
	int result=-4;
	
	if (buf[0]=='=') // if '=' at first index
		result=-1;
	
	else if (buf[0]>='0' && buf[0]<='9')
		result=-3;
	else
	{
		for(int i=0; i<len ;i++)
		{
			if( (buf[i]>='0' && buf[i]<='9') || (buf[i]>='a' && buf[i]<='z') || (buf[i]>='A' && buf[i]<='Z') || buf[i]=='_' );
			
			else if (buf[i]=='=') 
			{
				result=i;
				break;
			}

			else
			{
				//printf("entering wrong i=%d,last=%d, len=%d \n",i,buf[i],len);
				result=-5;
				break;
			}
		}
	}
	
	return result;
}




