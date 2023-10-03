#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"
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

 
    struct rusage r;
    int wait_pid = wait2(0, &r);
    uint end_time = uptime();

   if (wait_pid < 0){
   	fprintf(2, "Failed to wait for child process\n");
   	exit(1);
   }
   fprintf(1, "elapsed time: %d ticks, cpu time: %d ticks, %d%% CPU\n", end_time - start_time, r.cputime, (r.cputime * 100) / (end_time - start_time) );

    exit(0);
}

