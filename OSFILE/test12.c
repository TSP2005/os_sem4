
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100

struct ThreadData {
    int* arr;
    int start;
    int end;
};

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Function to perform merge sort recursively
void* merge_sort(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    int* arr = data->arr;
    int start = data->start;
    int end = data->end;

    if (start < end) {
        int mid = start + (end - start) / 2;

        struct ThreadData left_data = { arr, start, mid };
        struct ThreadData right_data = { arr, mid + 1, end };

        pthread_t left_thread, right_thread;

        pthread_create(&left_thread, NULL, merge_sort, &left_data);
        pthread_create(&right_thread, NULL, merge_sort, &right_data);

        pthread_join(left_thread, NULL);
        pthread_join(right_thread, NULL);

        merge(arr, start, mid, end);
    }

    pthread_exit(NULL);
}

int main() {
    int arr[MAX_SIZE];
    int n;

    printf("Enter the number of elements: ");
    scanf("%d", &n);

    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    struct ThreadData main_data = { arr, 0, n - 1 };
    pthread_t main_thread;
    pthread_create(&main_thread, NULL, merge_sort, &main_data);
    pthread_join(main_thread, NULL);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}
