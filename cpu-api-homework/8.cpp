#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <iostream>

int main() {
    // first child sends to second child
    int pipefd[2] = {};
    if (pipe(pipefd) == -1) {
        std::cerr << "create pipe failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto pidfirst = fork();
    if (pidfirst < 0) {
        std::cerr << "Failed to fork first child" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (pidfirst == 0) {
        close(pipefd[0]);
        std::string s = "Wrtie to second child";
        write(pipefd[1], s.c_str(), s.size());
        close(pipefd[1]);
        std::cout << "ㄅㄅ1" << std::endl;
        exit(EXIT_SUCCESS);
    }

    auto pidsecond = fork();
    if (pidsecond < 0) {
        std::cerr << "Failed to fork second child" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (pidsecond == 0) {
        close(pipefd[1]);
        char buf;
        while (read(pipefd[0], &buf, sizeof(buf)) > 0) {
            write(STDOUT_FILENO, &buf, sizeof(buf));
        }
        close(pipefd[0]);
        std::cout << "ㄅㄅ2" << std::endl;
        exit(EXIT_SUCCESS);
    }

    close(pipefd[0]);
    close(pipefd[1]); // the most parent process should close write end of the pipe
                      // or child that reads the pipe would be blocked
    waitpid(pidfirst, NULL, 0);
    waitpid(pidsecond, NULL, 0);
    std::cout << "first: " << pidfirst << "second: " << pidsecond << std::endl;
}