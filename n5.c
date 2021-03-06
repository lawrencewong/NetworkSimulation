/*
n5.c
This is leaf node that prints out the data it is given and reports back that it has recieved the message.
*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main()
{
	int fd;
	int awake = 1;
	char * link4 = "/tmp/link4";
	char packet[4];

	// Constanlty blocked on reading for data or the kill switch
	while(awake)
	{

		fd = open(link4, O_RDONLY);

		while( read(fd, packet, 4))
		{
			// Shuts down node if recieved killswitch packet
			if(packet[3] == 'F')
			{
				printf("[Node 5]Node 5 shutting down ...\n");
				awake = 0;
				break;
			}

			// Prints the data
			if(packet[2] == 'F')
			{
					printf("\n[Node 5]----- END OF MESSAGE -----\n");
					close(fd);
					fd = open(link4, O_WRONLY);
					write(fd, "Received by n5", sizeof("Received by n5"));
					close(fd);
					break;

			}
			else if(packet[2] == 'S')
			{
				printf("\n[Node 5]----- START OF MESSAGE -----\n");

			}
			if(packet[1] == '5')
			{
				printf("%c", packet[0]);
				fflush(stdout);
			}
		}
		
		if(!awake)
		{
			return 0;
		}
		
		fd = open(link4, O_RDONLY);
		read(fd, packet, 4);
		close(fd);
		if(packet[3] == 'F')
		{
			printf("[Node 5]Node 5 shutting down ...\n");
			awake = 0;
		}


	}
	return 0;
}