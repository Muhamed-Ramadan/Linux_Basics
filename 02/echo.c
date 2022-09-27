#include <stdio.h>



int main(int argc, char* argv[])
{
	int i=0;
	if (argc ==1) return -1;

	for (i=1;i<(argc-1);i++)
	{
		printf("%s ",argv[i]);
	}
	printf("%s\n",argv[i]);// to printf last item  without extra space at the end;
	return 0;
}
