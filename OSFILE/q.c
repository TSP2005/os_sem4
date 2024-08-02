#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <setjmp.h>

jmp_buf env;

void alarm_handler(int signum) {
    longjmp(env, 1);
}

int is_prime(int num) {
    if (num <= 1)
        return 0;
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

void find_primes(int x, int y) {
    for (int i = x; i <= y; ++i) {
        if (is_prime(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

int main() {
    int x, y, z;
    printf("Enter three numbers (x, y, z): ");
    scanf("%d %d %d", &x, &y, &z);

    signal(SIGALRM, alarm_handler);
    
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // child process
        find_primes(x, y);
        exit(EXIT_SUCCESS);
    } else { // parent process
        if (setjmp(env) == 0) {
            alarm(z);
            wait(NULL);
            printf("All the prime numbers printed\n");
            longjmp(env, 2);
        } else {
            printf("Calculation is taking too much time\n");
        }
    }

    return 0;
}
