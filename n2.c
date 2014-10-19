/*
n2.c
This is node sets up (link3) to communicate with node 4.
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
	int fdn4;
	int awake = 1;
	char * link1 = "/tmp/link1";
	char * link3 = "/tmp/link3";
	char packet[4];
	int sleepfor;
	char buf[MAX_BUF];

	// Creating link 3 that will talk to node 4
	mkfifo(link3, 0666);

	// Constatnly blocked on reading until it gets some packets.
	while(awake)
	{
		fd = open(link1, O_RDONLY);
		fdn4 = open(link3, O_WRONLY);
		packet[3] = 'T';
		
		while( read(fd, packet, 4))
		{

			// Recieving the kill switch packet and shutsdown child node.
			if(packet[3] == 'F')
			{
				close(fd);
				close(fdn4);
				printf("[Node 2]Node 2 shutting down ...\n");
				packet[0] = '!';
				packet[1] = '!';
				packet[2] = '!';
				packet[3] = 'F';
				fdn4 = open(link3, O_WRONLY);
				write(fdn4, packet, sizeof(packet));
				close(fdn4);			
				awake = 0;
				break;
			}

			// Printing out message or sending it to the child node.
			if(packet[2] == 'F' && packet[1] == '2')
			{
				printf("\n[Node 2]----- END OF MESSAGE -----\n");
				break;
			}
			else if(packet[2] == 'S' && packet[1] == '2')
			{
				printf("\n[Node 2]----- START OF MESSAGE -----\n");
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
				//printf("---OUT---: %c %c %c \n", packet[0], packet[1], packet[2]);
				write(fdn4, packet, sizeof(packet));
			}

		} 

		if(!awake)
		{
			continue;
		}


		close(fd);
		close(fdn4);

		// Sending the confirmation of a recieved message by a node to the parent.
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
			printf("[Node 2]---IN---: %s\n", buf);
			close(fdn4);
			fd = open(link1, O_WRONLY);
			write(fd, buf, sizeof(buf));
			close(fd);
		}

		// Checking to see if the system will end or not.
		fd = open(link1, O_RDONLY);
		read(fd, packet, 4);
		close(fd);
		if(packet[3] == 'F')
		{
			printf("[Node 2]Node 2 shutting down ...\n");
			packet[0] = '!';
			packet[1] = '!';
			packet[2] = '!';
			packet[3] = 'F';
			fdn4 = open(link3, O_WRONLY);
			write(fdn4, packet, sizeof(packet));
			close(fdn4);			
			awake = 0;
		}
	}


	unlink(link3);
	return 0;
}