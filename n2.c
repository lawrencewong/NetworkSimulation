#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define _BSD_SOURCE
#define MAX_BUF 1024

int main()
{
	int fd;
	int fdn4;
	int awake = 1;
	char * link1 = "/tmp/link1";
	char * link3 = "/tmp/link3";
	char packet[4];
	int sleepfor;
	char buf[MAX_BUF];

	mkfifo(link3, 0666);

	while(awake)
	{
		fd = open(link1, O_RDONLY);
		fdn4 = open(link3, O_WRONLY);
		packet[3] = 'T';
		while( read(fd, packet, 4))
		{

			if(packet[3] == 'F')
			{
				close(fd);
				close(fdn4);
				printf("Node 2 shutting down ...\n");
				packet[0] = '!';
				packet[1] = '!';
				packet[2] = '!';
				packet[3] = 'F';
				fdn4 = open(link3, O_WRONLY);
				write(fdn4, packet, sizeof(packet));
				fflush(stdout);
				close(fdn4);			
				awake = 0;
				break;
			}
			if(packet[2] == 'F' && packet[1] == '2')
			{
				printf("\n----- END OF MESSAGE -----\n");
				fflush(stdout);
				break;
			}
			else if(packet[2] == 'S' && packet[1] == '2')
			{
				printf("\n----- START OF MESSAGE -----\n");
				fflush(stdout);
			}

			if(packet[1] == '2')
			{
				printf("%c", packet[0]);	
				fflush(stdout);
			}
			else if(packet[1] == '4')
			{
				sleepfor = (rand() % 2000000) + 1000;
				usleep(sleepfor);
				printf("---OUT---: %c %c %c \n", packet[0], packet[1], packet[2]);
				
				write(fdn4, packet, sizeof(packet));
			}

		} 

		if(!awake)
		{
			continue;
		}


		close(fd);
		close(fdn4);

		if(packet[1] == '2')
		{
			fd = open(link1, O_WRONLY);
			write(fd, "Received by n2", sizeof("Received by n2"));
			close(fd);
		}
		else if(packet[1] == '4')
		{
			fdn4 = open(link3, O_RDONLY);
			read(fdn4, buf, MAX_BUF);
			close(fdn4);
			printf("---IN---: %s\n", buf);
			fd = open(link1, O_WRONLY);
			write(fd, buf, sizeof(buf));
			close(fd);
		}

		fd = open(link1, O_RDONLY);
		read(fd, packet, 4);
		close(fd);
		if(packet[3] == 'F')
		{
			printf("Node 2 shutting down ...\n");
			packet[0] = '!';
			packet[1] = '!';
			packet[2] = '!';
			packet[3] = 'F';
			fdn4 = open(link3, O_WRONLY);
			write(fdn4, packet, sizeof(packet));
			fflush(stdout);
			close(fdn4);			
			awake = 0;
		}
	}


	unlink(link3);
	return 0;
}