#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <chrono>

void do_command(char** argv)
{
	auto start = std::chrono::steady_clock::now();
	pid_t child = fork();

        if(child == -1) 
	{
		printf("Error occurred: %s\n", strerror(errno));
                exit(errno);
	}

	if(child == 0)
	{
		if(execvp(argv[0], argv) == -1)
		{
			printf("Error accurred: %s\n", strerror(errno));
			exit(errno);
		} 
	}
	else
	{
		int status;
		if(waitpid(child, &status, 0) == -1)
		{
			printf("Error accurred: %s\n", strerror(errno));
                        exit(errno);
		}

		if(WIFEXITED(status))
		{
			auto end = std::chrono::steady_clock::now();
        		std::chrono::duration<double> elapsed_seconds = end - start;
			std::cout << argv[0] << " completed with " << WEXITSTATUS(status) << " exit code and took " << elapsed_seconds.count() << " seconds." << std::endl;
		}
		else
		{
			printf("child process did not terminate normally\n");
		}
	}
}

int main(int argc, char** argv)
{
	if (argc < 2) 
	{
		printf("File name was not given by arguments\n");
		exit(1);
	}

	do_command(argv+1);
}

