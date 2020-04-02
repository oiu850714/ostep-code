#include <sys/time.h> // gettimeofday
#include <unistd.h> // read pipe
#include <iostream> // std::cout

#include <sched.h>
// man sched_setaffinity

double getEstimatedContextSwitchTime(const timeval &start, const timeval &end, const int iteration) {
    return ((1000000 * end.tv_sec + end.tv_usec) - (1000000 * start.tv_sec + start.tv_usec)) / iteration;
}

int main() {
    // From OSTEP:
    // The first process then issues a write to the first pipe, and waits for a read on the second;
    // upon seeing the first process waiting for something to read from
    // the second pipe, the OS puts the first process in the blocked state, and
    // switches to the other process, which reads from the first pipe and then
    // writes to the second. When the second process tries to read from the first
    // pipe again, it blocks, and thus the back-and-forth cycle of communication
    // continues. By measuring the cost of communicating like this repeatedly,
    // lmbench can make a good estimate of the cost of a context switch.
    int firstPipeFd[2]{}, secondPipeFd[2]{};

    // omit error check
    pipe(firstPipeFd);
    pipe(secondPipeFd);

    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);

    constexpr auto iteration = 1000000;
    auto childPid = fork();
    if (childPid < 0) {
        std::cerr << "fork failed" << std::endl;
    } else if (childPid == 0) {
        close(firstPipeFd[1]);
        close(secondPipeFd[0]);
        sched_setaffinity(0, sizeof(cpu_set_t), &set);
        for (auto i = 0; i < iteration; i++) {
            read(firstPipeFd[0], nullptr, 0);
            write(secondPipeFd[1], nullptr, 0);
        }

    } else {
        close(firstPipeFd[0]);
        close(secondPipeFd[1]);
        sched_setaffinity(getpid(), sizeof(cpu_set_t), &set);
        struct timeval start{}, end{};
        gettimeofday(&start, nullptr);
        for (auto i = 0; i < iteration; i++) {
            write(firstPipeFd[1], nullptr, 0);
            read(secondPipeFd[1], nullptr, 0);
        }
        gettimeofday(&end, nullptr);
        std::cout << "Estimated context switch overhead: " << getEstimatedContextSwitchTime(start, end, iteration) << "us" << std::endl;
    }
    // 不確定:
    // 1. 讀寫 0 bytes 真的會觸發 context switch 嗎? 但讀寫 1 bytes，算出來的時間又會包含真的 I/O
    // 2. 題目說，parent「先寫 pipe1 再讀 pipe2」，child「先"讀" pipe1 再"寫" pip2」，
    //        感覺兩個 processes 如果都改成先寫再讀，應該不會有差異，但是不確定
}