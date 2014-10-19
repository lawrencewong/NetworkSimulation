#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int fd;
	int awake = 1;
	char * link3 = "/tmp/link3";
	char packet[4];

	while(awake)
	{

		fd = open(link3, O_RDONLY);

		while( read(fd, packet, 4))
		{
			fflush(stdout);
			if(packet[3] == 'F')
			{
				printf("[Node 4]Node 4 shutting down ...\n");
				fflush(stdout);
				awake = 0;
				break;
			}
			if(packet[2] == 'F')
			{
					printf("\n[Node 4]----- END OF MESSAGE -----\n");
					close(fd);
					fd = open(link3, O_WRONLY);
					write(fd, "Received by n4", sizeof("Received by n4"));
					close(fd);

					break;

			}
			else if(packet[2] == 'S')
			{
				printf("\n[Node 4]----- START OF MESSAGE -----\n");

			}
			if(packet[1] == '4')
			{
				printf("%c", packet[0]);
				fflush(stdout);
			}
		}
		
		if(!awake)
		{
			return 0;
		}
		
		fd = open(link3, O_RDONLY);
		read(fd, packet, 4);
		close(fd);
		if(packet[3] == 'F')
		{
			printf("[Node 4]Node 4 shutting down ...\n");
			awake = 0;
		}


	}
	return 0;
}