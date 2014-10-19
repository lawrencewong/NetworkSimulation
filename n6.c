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
	char * link5 = "/tmp/link5";
	char packet[4];

	while(awake)
	{

		fd = open(link5, O_RDONLY);

		while( read(fd, packet, 4))
		{
			fflush(stdout);
			if(packet[3] == 'F')
			{
				printf("Node 6 shutting down ...\n");
				fflush(stdout);
				awake = 0;
				break;
			}
			if(packet[2] == 'F')
			{
					printf("\n----- END OF MESSAGE -----\n");
					close(fd);
					fd = open(link5, O_WRONLY);
					write(fd, "Received by n6", sizeof("Received by n4"));
					close(fd);

					break;

			}
			else if(packet[2] == 'S')
			{
				printf("\n----- START OF MESSAGE -----\n");

			}
			if(packet[1] == '6')
			{
				printf("%c", packet[0]);
				fflush(stdout);
			}
		}
		
		if(!awake)
		{
			return 0;
		}
		
		fd = open(link5, O_RDONLY);
		read(fd, packet, 4);
		close(fd);
		if(packet[3] == 'F')
		{
			printf("Node 6 shutting down ...\n");
			awake = 0;
		}


	}
	return 0;
}