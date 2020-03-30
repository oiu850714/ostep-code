#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <cstring> // strdup

extern char** environ;

int exec1();
int exec2();
int exec3();
int exec4();
int exec5();
int exec6();



int main() {
    auto pid = fork();
    if (pid < 0) {
        std::cerr << "Failed to fork" << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        exec1();
        // exec2();
        // exec3();
        // exec4();
        // exec5();
        // exec6();
    } else {
        wait(NULL);
        std::cout << "child finished" << std::endl;
    }
}


int exec1() {
    return execl("/bin/ls", "/bin/ls", "-al", NULL);
}

int exec2() {
    return execle("/bin/ls", "/bin/ls", "-al", NULL, environ);
}

int exec3() {
    return execlp("ls", "/bin/ls", "-al", NULL);
}

int exec4() {
    char *args[3] {strdup("/bin/ls"), strdup("-al"), NULL};
    return execv(args[0], args);
}

int exec5() {
    char *args[3] {strdup("/bin/ls"), strdup("-al"), NULL};
    return execvp(args[0], args);
}

int exec6() {
    char *args[3] {strdup("/bin/ls"), strdup("-al"), NULL};
    return execvpe(args[0], args, environ);
}