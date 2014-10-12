#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define _BSD_SOURCE

#define MAX_BUF 2

int main()
{
	int fd;
	char systemSwitch[1];
	int awake = 1;
	char * link1 = "/tmp/link1";
	char packet[3];

	while(awake)
	{
		fd = open(link1, O_RDONLY);
		

		while( read(fd, packet, 3))
		{
			if(packet[2] == 'F')
			{
				printf("\n----- END OF MESSAGE -----\n");
				break;
			}
			else if(packet[2] == 'S')
			{
				printf("\n----- START OF MESSAGE -----\n");
			}

			if(packet[1] == '2')
			{
				printf("%c", packet[0]);	
				fflush(stdout);
			}
		} 

		close(fd);

		printf("\n");
		if(packet[1] == '2')
		{
			fd = open(link1, O_WRONLY);
			write(fd, "Received by n2", sizeof("Received by n2"));
			close(fd);
		}

		fd = open(link1, O_RDONLY);
		read(fd, systemSwitch, 1);
		if(systemSwitch[0] == '0')
		{
			// TODO shutdown node 4
			printf("Node 2 shutting down ...\n");			
			awake = 0;
		}
	}
	return 0;
}