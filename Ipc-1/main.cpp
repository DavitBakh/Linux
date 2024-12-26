#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <pwd.h>

using namespace std;

void signalHandler(int signo, siginfo_t* info, void* context) 
{
    if (signo == SIGUSR1) 
    {
        pid_t pid = info->si_pid;
        uid_t uid = info->si_uid;

        struct passwd *pwd = getpwuid(uid);
        const char *username = pwd ? pwd->pw_name : "unknown";

        std::cout << "Received a SIGUSR1 signal from process " << pid << " executed by UID " << uid << " (" << username << ").\n";

        ucontext_t* uc = (ucontext_t*)context;
        #if defined(__x86_64__)
        auto rip = uc->uc_mcontext.gregs[REG_RIP];
        auto rax = uc->uc_mcontext.gregs[REG_RAX];
        auto rbx = uc->uc_mcontext.gregs[REG_RBX];
        std::cout << "State of the context: RIP = " << std::hex << rip << ", RAX = " << std::hex << rax << ", RBX = " << std::hex << rbx << ".\n";
        #elif defined(__i386__)
        auto eip = uc->uc_mcontext.gregs[REG_EIP];
        auto eax = uc->uc_mcontext.gregs[REG_EAX];
        auto ebx = uc->uc_mcontext.gregs[REG_EBX];
        std::cout << "State of the context: EIP = " << std::hex << eip << ", EAX = " << std::hex << eax << ", EBX = " << std::hex << ebx << ".\n";
        #else
        std::cerr << "Unsupported architecture for accessing registers.\n";
        #endif
    }
}

int main() 
{
    cout << "PID: " << getpid() << endl;

    struct sigaction action{};
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = signalHandler;

    if (sigaction(SIGUSR1, &action, nullptr) == -1) 
    {
        perror("sigaction");
        return 1;
    }

    while (true) 
    {
        std::cout << "Waiting for SIGUSR1..." << std::endl;
        sleep(10);
    }

    return 0;
}
