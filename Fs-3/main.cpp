#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

int main(int argc, char* argv[])
{
	if (argc < 2) 
	{
		printf("File name was not given by arguments\n");
		exit(1);
	}

	struct stat fileStat;
	if (stat(argv[1], &fileStat) != 0)
	{
		printf("Error: Unable to access file: %s\n", strerror(errno));
		return 1;
	}

	int fd = open(argv[1], O_WRONLY);

	if (fd == -1)
	{
		printf("Something went wrong while opening the source file. Error %s\n", strerror(errno));
		exit(errno);
	}

	char null = '\0';
	off_t size = lseek(fd, 0, SEEK_END);

	for (off_t i = 0; i < size; i++)
	{
		if (write(fd, &null, 1) != 1)
		{
			printf("Error: Unable to write to the file: %s\n", strerror(errno));
			close(fd);
			exit(errno);
		}
	}

	close(fd);

	if (unlink(argv[1]) != 0) 
	{
		printf("Error: Unable to delete the file: %s\n", strerror(errno));
		exit(errno);
	}

	printf("File deleted\n");

	return 0;
}
