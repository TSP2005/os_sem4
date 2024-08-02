#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <setjmp.h>

jmp_buf e;

void ah(int z) {
    longjmp(e, 1);
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

void main() {
    int x, y, z;
    printf("Enter three numbers (x, y, z): ");
    scanf("%d %d %d", &x, &y, &z);

    signal(SIGALRM, ah);

    int id = fork();

    if (id<0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (id == 0) {
        for (int i = x; i <= y; ++i) {
        if (is_prime(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");
        exit(EXIT_SUCCESS);
    } else {
        if (setjmp(e) == 0) {
            alarm(z);
            wait(NULL);
            printf("all the prime numbers printed\n");
            longjmp(e, 2);
        } else {
            printf("calculation is taking too much time\n");
        }
    }

}

