#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define _BSD_SOURCE
#define MAX_BUF 1024

int main()
{
	int fd;
	int sendAgain = 1;
	char confirm;
	char * link1 = "/tmp/link1";
	char * link2 = "/tmp/link2";
	FILE * filePointer;
	char buf[MAX_BUF];
	char filename[MAX_BUF];
	char destNode[MAX_BUF];
	int ch = 0;
	char packet[3];
	char end[1];

	// int mid = 1000000;
	int range;
	int sleepfor;

	srand(time(NULL));
	range = (2000000000 - 1000) + 1000;
	sleepfor = rand() % range + 1000;
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
		packet[1] = destNode[1];

		// Opening text file
		filePointer = fopen(filename, "r");
		if(filePointer == NULL){
			printf("Something went wrong opening that file.\n");
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
				printf("Invalid node destination.\n");
				continue;
			}
			
			while((ch = fgetc(filePointer)) != EOF)
			{
				packet[0] = ch;
				packet[2] = 'T';
				usleep(1000000);
				printf("---OUT---: %c %c %c \n", packet[0], packet[1], packet[2]);
				write(fd, packet, sizeof(packet));
			} 
			packet[2] = 'F';
			write(fd, packet, sizeof(packet));
			close(fd);


			fd = open(link1, O_RDONLY);
			read(fd, buf, MAX_BUF);
			close(fd);
			printf("---IN---: %s\n", buf);
			printf("Would you like to continue? (Y/N)\n");
			scanf("%c",&confirm);
			getchar();
			if(confirm == 'N')
			{
				printf("Shutting down all nodes...\n");
				sendAgain = 0;
				end[0] = '0';
			}
			else
			{
				end[0] = '1';
			}
			fd = open(link1, O_WRONLY);
			write(fd, end, sizeof(end));
			close(fd);
		}
		
	}	

	

	unlink(link1);
	unlink(link2);

	return 0;
}