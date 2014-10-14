#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define _BSD_SOURCE

int main()
{
	int fd;
	int awake = 1;
	char * link3 = "/tmp/link3";
	char packet[3];
	char systemSwitch[1];

	while(awake)
	{

		fd = open(link3, O_RDONLY);

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
			if(packet[1] == '4')
			{
				printf("%c", packet[0]);
				fflush(stdout);
			}
		}
		close(fd);

		fd = open(link3, O_WRONLY);
		write(fd, "Received by n4", sizeof("Received by n4"));
		close(fd);

		fd = open(link3, O_RDONLY);
		read(fd, systemSwitch, 1);
		close(fd);
		if(systemSwitch[0] == '0')
		{
			printf("Node 4 shutting down ...\n");
			awake = 0;
		}


	}
	return 0;
}