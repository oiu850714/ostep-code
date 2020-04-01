#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

int main() {
    auto pid = fork();
    if (pid < 0) {
        std::cerr << "fork failed" << std::endl;
    } else if (pid == 0) {
        auto child_wait_val = wait(NULL);
        // return -1
        if (child_wait_val == -1) {
            perror("child wait error: ");
        }
        std::cout << "in child, wait return: " << child_wait_val << std::endl;
        return EXIT_SUCCESS;
    } else {
        auto parent_wait_val = waitpid(pid, NULL, 0);
        std::cout << "in parent, wait return: " << parent_wait_val << std::endl;
        return EXIT_SUCCESS;
    }
}