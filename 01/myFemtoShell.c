#include <stdio.h>
#include <string.h>

int main ()
{
	char arr[100]="0";
	while(1)
	{
		
		printf("Ana Gahez ya Basha > ");
		scanf("%s\\0",arr);
		if ( strcmp(arr,"exit") )
		{
			printf("you said: %s\n",arr);
		}
		else
		{
			printf("Good Bye :)\n");
			break;
		}
	}
	
	return 0;
}
