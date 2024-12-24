#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include <fcntl.h>

using namespace std;

void DoCommand(string& cmd, bool silent)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        cout << "Error: " << strerror(errno) << endl;
        return;
    }

    if (pid == 0)
    {
        std::string path = getenv("PATH");
        path = "./:" + path;
        setenv("PATH", path.c_str(), 1);

        cmd = cmd.substr(silent ? 7 : 0);
        if(silent)
        {
            string outputFile = std::to_string(getpid()) + ".log";
            int fd = open(outputFile.c_str(), O_WRONLY | O_CREAT, 0644);
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
        }

        char* args[64];
        char *token = strtok(&cmd[0], " ");

        int i = 0;
        while (token != nullptr) {
            args[i++] = token;
            token = strtok(nullptr, " ");
        }
        args[i] = nullptr;

        execvp(args[0], args);
        std::cerr << "Command execution failed." << std::endl;
        exit(1);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
}

int main(int argc, char* argv[])
{
    while (1)
    {
        string cmd;
        getline(cin, cmd);

        if(cmd.empty())
            continue;

        if(cmd == "exit")
            return 0;

        bool silent = cmd.find("silent ") == 0;
        DoCommand(cmd, silent);
    }
    
    return 0;
}