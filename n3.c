/*
n3.c
This is node sets up (link4, link5, link6) to communicate with node 5, 6, 7 respectivly.
This node can print the data that it is meant to or pass it along
to node 4 with some time delay.
*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#define _BSD_SOURCE
#define MAX_BUF 1024

int main()
{
	int fd;
	int fdn5;
	int fdn6;
	int fdn7;
	int awake = 1;
	char * link2 = "/tmp/link2";
	char * link4 = "/tmp/link4";
	char * link5 = "/tmp/link5";
	char * link6 = "/tmp/link6";
	char packet[4];
	int sleepfor;
	char buf[MAX_BUF];

	// Creating links that will talk to it's child nodes.
	mkfifo(link4, 0666);
	mkfifo(link5, 0666);
	mkfifo(link6, 0666);

	// Constatnly blocked on reading until it gets some packets.
	while(awake)
	{
		fd = open(link2, O_RDONLY);
		fdn5 = open(link4, O_WRONLY);
		fdn6 = open(link5, O_WRONLY);
		fdn7 = open(link6, O_WRONLY);
		packet[3] = 'T';

		while( read(fd, packet, 4))
		{

			// Recieving the kill switch packet and shutsdown child node.
			if(packet[3] == 'F')
			{
				close(fd);
				close(fdn5);
				close(fdn6);
				close(fdn7);
				printf("[Node 3]Node 3 shutting down ...\n");
				packet[0] = '!';
				packet[1] = '!';
				packet[2] = '!';
				packet[3] = 'F';
				fdn5 = open(link4, O_WRONLY);
				write(fdn5, packet, sizeof(packet));
				close(fdn5);
				fdn6 = open(link5, O_WRONLY);
				write(fdn6, packet, sizeof(packet));
				close(fdn6);	
				fdn7 = open(link6, O_WRONLY);
				write(fdn7, packet, sizeof(packet));
				close(fdn7);			
				awake = 0;
				break;
			}

			// Printing out message or sending it to the child node.
			if(packet[2] == 'F' && packet[1] == '3')
			{
				printf("\n[Node 3]----- END OF MESSAGE -----\n");
				break;
			}
			else if(packet[2] == 'S' && packet[1] == '3')
			{
				printf("\n[Node 3]----- START OF MESSAGE -----\n");
			}

			if(packet[1] == '3')
			{
				printf("%c", packet[0]);	
				fflush(stdout);
			}
			else if(packet[1] == '5')
			{ 
				sleepfor = (rand() % 2000000) + 1000;
				usleep(sleepfor);
				//printf("---OUT---: %c %c %c \n", packet[0], packet[1], packet[2]);
				write(fdn5, packet, sizeof(packet));
			}
			else if(packet[1] == '6')
			{
				sleepfor = (rand() % 2000000) + 1000;
				usleep(sleepfor);
				//printf("---OUT---: %c %c %c \n", packet[0], packet[1], packet[2]);
				write(fdn6, packet, sizeof(packet));
			}
			else if(packet[1] == '7')
			{
				sleepfor = (rand() % 2000000) + 1000;
				usleep(sleepfor);
				//printf("---OUT---: %c %c %c \n", packet[0], packet[1], packet[2]);
				write(fdn7, packet, sizeof(packet));
			}

		} 

		if(!awake)
		{
			continue;
		}

		close(fd);
		close(fdn5);
		close(fdn6);
		close(fdn7);

		// Sending the confirmation of a recieved message by a node to the parent.
		if(packet[1] == '3')
		{
			fd = open(link2, O_WRONLY);
			write(fd, "Received by n3", sizeof("Received by n3"));
			close(fd);
		}
		else if(packet[1] == '5')
		{
			fdn5 = open(link4, O_RDONLY);
			read(fdn5, buf, MAX_BUF);
			printf("[Node 3]---IN---: %s\n", buf);
			close(fdn5);
			fd = open(link2, O_WRONLY);
			write(fd, buf, sizeof(buf));
			close(fd);
		}
		else if(packet[1] == '6')
		{
			fdn6 = open(link5, O_RDONLY);
			read(fdn6, buf, MAX_BUF);
			printf("[Node 3]---IN---: %s\n", buf);
			close(fdn6);
			fd = open(link2, O_WRONLY);
			write(fd, buf, sizeof(buf));
			close(fd);
		}
		else if(packet[1] == '7')
		{
			fdn7 = open(link6, O_RDONLY);
			read(fdn7, buf, MAX_BUF);
			printf("[Node 3]---IN---: %s\n", buf);
			close(fdn7);
			fd = open(link2, O_WRONLY);
			write(fd, buf, sizeof(buf));
			close(fd);
		}

		// Checking to see if the system will end or not.
		fd = open(link2, O_RDONLY);
		read(fd, packet, 4);
		close(fd);
		if(packet[3] == 'F')
		{
			printf("[Node 3]Node 3 shutting down ...\n");
			packet[0] = '!';
			packet[1] = '!';
			packet[2] = '!';
			packet[3] = 'F';
			fdn5 = open(link4, O_WRONLY);
			write(fdn5, packet, sizeof(packet));
			close(fdn5);
			fdn6 = open(link5, O_WRONLY);
			write(fdn6, packet, sizeof(packet));
			close(fdn6);	
			fdn7 = open(link6, O_WRONLY);
			write(fdn7, packet, sizeof(packet));
			close(fdn7);			
			awake = 0;
		}
	}

	unlink(link4);
	unlink(link5);
	unlink(link6);
	return 0;
}