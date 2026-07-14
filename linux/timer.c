#include <stddef.h>
#include <sys/time.h>
#include "rtos.h"

void tick_handler(int sig) {
        (void)sig;
        system_ticks++;
}

void timer_init() {
        // register handle
        struct sigaction sa;
        sa.sa_handler = tick_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sigaction(SIGALRM, &sa, NULL);

        // arm timer
        struct itimerval timer;
        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = 10000;
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = 10000;
        setitimer(ITIMER_REAL, &timer, NULL);
}


