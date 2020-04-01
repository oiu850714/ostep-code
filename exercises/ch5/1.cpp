#include <unistd.h>
#include <iostream>

int main() {
    int val = 9487;
    auto pid = fork();
    if (pid < 0) {
        std::cerr << "fork failed" << std::endl;
    } else if (pid == 0) {
        std::cout << "in child, val = " << val << std::endl;
        return EXIT_SUCCESS;
    } else {
        std::cout << "in parent, val = " << val << std::endl;
        return EXIT_SUCCESS;
    }
}