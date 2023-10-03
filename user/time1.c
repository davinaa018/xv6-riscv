#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(2, "Usage: time1 <command>\n");
        exit(1);
    }

    uint start_time = uptime();
    int pid = fork();

    if (pid == 0) {
        // In the child process, execute the specified command
        exec(argv[1], argv + 1);  // Pass all arguments to the command except the program name
        fprintf(2, "Failed to execute %s\n", argv[1]);
        exit(1);
    } else if (pid == -1) {
        fprintf(2, "Fork failed\n");
        exit(1);
    }

    int status;
    wait(&status);
    uint end_time = uptime();

    if (status == 0) {
        printf("elapsed time: %d ticks\n", end_time - start_time);
    } else {
        fprintf(2, "Command failed\n");
    }

    exit(0);
}

