#include <sys/time.h> // gettimeofday
#include <unistd.h> // read
#include <vector> // std::vector
#include <utility> // std::pair
#include <iostream> // std::cout

double getEstimatedSyscallTime(const timeval &start, const timeval &end, const int iteration) {
    return ((1000000 * end.tv_sec + end.tv_usec) - (1000000 * start.tv_sec + start.tv_usec)) / iteration;
}

int main() {
    struct timeval start{}, end{};
    constexpr int iteration = 1000000;
    gettimeofday(&start, nullptr);
    for (int i = 0; i < iteration; i++) {
        read(STDIN_FILENO, nullptr, 0);
    }
    gettimeofday(&end, nullptr);
    std::cout << "Estimated syscall overhead: " << getEstimatedSyscallTime(start, end, iteration) << "us" << std::endl;
}