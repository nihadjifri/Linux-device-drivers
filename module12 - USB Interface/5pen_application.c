#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<signal.h>
void sig_handler(int signo)
{
	printf("Application received a signal : %d\n",signo);
}
int main()
{
	signal(SIGPIPE,sig_handler);
	int fd = open("/dev/pen0",O_RDWR);
//	int fd = open("./file.temp",O_RDWR);
//	int fd = open("./test",O_WRONLY);
	if(fd == -1)
	{
		perror("open failed :");
		exit(1);
	}
	else
	{
		printf("open successfully fd : %d\n",fd);
		char buff[5]="hello";
	/*	int w = write(fd,buff,sizeof(buff));
		if(w == -1)
		{
			perror("write failed :");
			exit(1);
		}
		else
		{
			printf("write successfull\n");
		}*/
	/*	int off = lseek(fd,-5,SEEK_END);
		if(off == -1)
		{
			perror("lseek failed : ");
		}
		printf("ofset : %d\n",off);
	*/	//lseek(fd,3,SEEK_CUR);
//		lseek(fd,-5,SEEK_END);
		char rbuff[5];
		
		int r = read(fd,rbuff,sizeof(rbuff));
		if(r == -1)
		{
			perror("read failed :");
			exit(1);
		}
		else
		{
			rbuff[r]='\0';
			printf("data read successfull : %s\n",rbuff);
		}
	}
}
