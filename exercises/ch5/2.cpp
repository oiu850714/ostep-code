#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>

void write100kchar(int fd, const std::string &str) {
    int i = 100000;
    while (i--) {
        auto wLen = write(fd, str.c_str(), str.size());
        if (wLen != str.size()) {
            std::cerr << "write failed" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

int main() {
    auto fd = open("/tmp/file.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd <= 0) {
        std::cout << "open file failed" << fd << std::endl;
        return EXIT_FAILURE;
    }
    auto pid = fork();
    if (pid < 0) {
        std::cerr << "fork failed" << std::endl;
    } else if (pid == 0) {
        write100kchar(fd, "1234\n");
        close(fd);
        return EXIT_SUCCESS;
    } else {
        write100kchar(fd, "5678\n");
        close(fd);
        return EXIT_SUCCESS;
    }
}