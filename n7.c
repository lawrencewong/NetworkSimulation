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
	char * link6 = "/tmp/link6";
	char packet[4];

	while(awake)
	{

		fd = open(link6, O_RDONLY);

		while( read(fd, packet, 4))
		{
			if(packet[3] == 'F')
			{
				printf("[Node 7]Node 7 shutting down ...\n");
				awake = 0;
				break;
			}
			if(packet[2] == 'F')
			{
					printf("\n[Node 7]----- END OF MESSAGE -----\n");
					close(fd);
					fd = open(link6, O_WRONLY);
					write(fd, "Received by n7", sizeof("Received by n7"));
					close(fd);
					break;

			}
			else if(packet[2] == 'S')
			{
				printf("\n[Node 7]----- START OF MESSAGE -----\n");

			}
			if(packet[1] == '7')
			{
				printf("%c", packet[0]);
				fflush(stdout);
			}
		}
		
		if(!awake)
		{
			return 0;
		}
		
		fd = open(link6, O_RDONLY);
		read(fd, packet, 4);
		close(fd);
		if(packet[3] == 'F')
		{
			printf("[Node 7]Node 7 shutting down ...\n");
			awake = 0;
		}


	}
	return 0;
}