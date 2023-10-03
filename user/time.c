#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"
#include <stddef.h>


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: time <command>\n");
        exit(0);
    }

    struct rusage r;

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "Fork failed\n");
        exit(1);
    }

    if (pid == 0) {
        exec(argv[1], argv + 1);
        fprintf(2, "Execution of %s failed\n", argv[1]);
        exit(1);
    } else {
        int start_ticks = uptime();
        int wait_pid = wait2(0, &r);
        int end_ticks = uptime();

        if (wait_pid < 0) {
            fprintf(2, "Failed to wait for child process\n");
            exit(1);
        }

        fprintf(1, "Time: %d ticks\n", end_ticks - start_ticks);
        fprintf(1, "elapsed time: %d ticks, cpu time: %d ticks, %d%% CPU\n", end_ticks - start_ticks, r.cputime, (r.cputime * 100) / (end_ticks - start_ticks));
    }

    exit(0);
}
