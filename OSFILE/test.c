#include <stdio.h>
#include <pthread.h>

int sum;

void *runner(void *param);

int main(int argc, char *argv[]) {
    pthread_t tid;
    pthread_attr_t attr; // Corrected type name

    if (argc != 2) {
        perror("Usage: ./program <positive_integer>");
        return -1;
    }

    if (atoi(argv[1]) < 0) {
        perror("Error: Please provide a positive integer.");
        return -1;
    }

    pthread_attr_init(&attr); // Initialize thread attributes
    pthread_create(&tid, &attr, runner, argv[1]);
    pthread_join(tid, NULL);

    printf("%d\n", sum);
    return 0;
}

void *runner(void *param) {
    int i, upper = atoi(param);
    sum = 0;
    for (i = 1; i <= upper; i++) {
        sum += i;
    }
    pthread_exit(0); // Corrected function name
}
