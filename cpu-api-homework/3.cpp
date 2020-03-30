#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <iostream>
#include <string>

void printBye(int sig) {
    std::cout << "bye!" << std::endl;
}

int main() {
    auto parent_pid = getpid();
    auto pid = fork();
    if (pid < 0) {
        std::cerr << "fork failed" << std::endl;
    } else if (pid == 0) {
        kill(parent_pid, SIGCONT);
        std::cout << "hello ㄏㄏ" << std::endl;
        return EXIT_SUCCESS;
    } else {
        struct sigaction act{}; // need to value initialize or UB
        act.sa_handler = printBye;
        if (sigaction(SIGCONT, &act, NULL)) {
            std::cerr << "sigaction failed" << std::endl;
            return 0;
        }
        pause();
    }
}