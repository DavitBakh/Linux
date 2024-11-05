#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("Please provide source and destination files\n");
		exit(1);
	}

	int sourceFd = open(argv[1], O_RDONLY);

	if (sourceFd == -1)
	{
		printf("Something went wrong while opening the source file. Error %s\n", strerror(errno));
		exit(errno);
	}

	int destinationFd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);

	if (destinationFd == -1) 
	{
		printf("Something went wrong while opening the destination file. Error %s\n", strerror(errno));
		exit(errno);
	}

	char* buffer = (char*)malloc(BUFFER_SIZE);
	long overallBytes = 0;
	long readBytes = 0;

	while (true)
	{
		readBytes = read(sourceFd, buffer, BUFFER_SIZE);

		if (readBytes == -1) 
		{
			printf("Something went wrong while reading the file. Error %s\n", strerror(errno));
			exit(errno);
		}

		if (readBytes == 0)
			break;

		overallBytes += readBytes;

		long writeBytes = write(destinationFd, buffer, (size_t)readBytes);

		if (writeBytes == -1) 
		{
			printf("Something went wrong while writing to destination file. Error %s\n", strerror(errno));
			exit(errno);
		}

	}

	close(sourceFd);
	close(destinationFd);

	free(buffer);

	printf("Completed copying file. Overall bytes copied: %ld\n", overallBytes);

	return 0;
}