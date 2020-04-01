#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <iostream>

int main() {
    auto pid = fork();
    if (pid < 0) {
        std::cerr << "fork failed" << std::endl;
    } else if (pid == 0) {
        close(STDOUT_FILENO);
        std::cout << "in child" << std::endl;
        if (std::cout.fail()) {
            std::cerr << "std::cout has error" << std::endl;
        }
        return EXIT_SUCCESS;
    } else {
        auto parent_wait_val = waitpid(pid, NULL, 0);
        std::cout << "in parent, wait return: " << parent_wait_val << std::endl;
        return EXIT_SUCCESS;
    }
}