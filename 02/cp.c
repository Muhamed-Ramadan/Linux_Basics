#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char* argv[])
{
	if (argc !=3)
	{
		printf("directory of source, destination or both are not correct\n");	
		return -1;
	}
	int count;
	char buf[100];
	int fdr=open(argv[1],O_RDONLY);
	int fdw=open(argv[2],O_WRONLY | O_CREAT);
	
	if(fdr<0)
	{
		printf("No such file or directory\n");
		return -1;
	}
	if (fdw<0)
	{
		printf("Destination directory can not be accessed\n");
		return -1;
	}
	count =read(fdr,buf,100);
	while (count)
	{
		write(fdw,buf,count);
		count =read(fdr,buf,100);	
	}
	close(fdr);
	close(fdw);
	
	
	return 0;
}
