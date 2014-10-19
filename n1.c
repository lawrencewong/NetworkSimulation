#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define _BSD_SOURCE
#define MAX_BUF 1024

int main()
{
	int fd;
	int fdn2;
	int fdn3;
	int sendAgain = 1;
	char confirm;
	char * link1 = "/tmp/link1";
	char * link2 = "/tmp/link2";
	FILE * filePointer;
	char buf[MAX_BUF];
	char filename[MAX_BUF];
	char destNode[MAX_BUF];
	int ch = 0;
	char packet[4];
	int sleepfor;

	// Creating links
	mkfifo(link1, 0666);
	mkfifo(link2, 0666);

	while(sendAgain)
	{
		// Get text file and node destination
		printf("Enter a text file and a destination node to forward the message to.\n");
		printf("Usage: <filename.txt> <node> EX. 'test.txt n4'\n");
		scanf("%s %s", filename, destNode);
		getchar();
		packet[0] = '!';
		packet[1] = destNode[1];
		packet[2] = '!';
		packet[3] = 'T';

		// Opening text file
		filePointer = fopen(filename, "r");
		if(filePointer == NULL){
			printf("[Node 1]Something went wrong opening that file.\n");
		}
		else
		{

			if(destNode[1] == '2' || destNode[1] == '4')
			{
				fd = open(link1, O_WRONLY);
			}
			else if(destNode[1] == '3' || destNode[1] == '5' || destNode[1] == '6' || destNode[1] == '7')
			{
				fd = open(link2, O_WRONLY);
			}
			else
			{
				printf("[Node 1]Invalid node destination.\n");
				continue;
			}
			
			packet[2] = 'S';

			while((ch = fgetc(filePointer)) != EOF)
			{
				packet[0] = ch;
				sleepfor = (rand() % 2000000) + 1000;
				usleep(sleepfor);
				//printf("---OUT---: %c %c %c %c\n", packet[0], packet[1], packet[2],packet[3]);
				write(fd, packet, sizeof(packet));
				packet[2] = 'T';
			} 
			packet[2] = 'F';
			write(fd, packet, sizeof(packet));
			close(fd);

			if(destNode[1] == '2' || destNode[1] == '4')
			{
				fd = open(link1, O_RDONLY);
			}
			else if(destNode[1] == '3' || destNode[1] == '5' || destNode[1] == '6' || destNode[1] == '7')
			{
				fd = open(link2, O_RDONLY);
			}
			read(fd, buf, MAX_BUF);
			printf("[Node 1]---IN---: %s\n", buf);
			close(fd);
			printf("[Node 1]Would you like to continue? (Y/N)\n");
			scanf("%c",&confirm);
			getchar();
			if(confirm == 'N')
			{
				printf("[Node 1]Shutting down all nodes...\n");
				sendAgain = 0;
				packet[3] = 'F';
			}
			packet[0] = '!';
			packet[1] = '!';
			packet[2] = '!';
			fdn2 = open(link1, O_WRONLY);
			write(fdn2, packet, sizeof(packet));
			close(fdn2);
			fdn3 = open(link2, O_WRONLY);
			write(fdn3, packet, sizeof(packet));
			close(fdn3);
		}
		
	}	

	

	unlink(link1);
	unlink(link2);

	return 0;
}