#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
struct Thd {
    int* arr;
    int start;
    int end;
};
void bsort(int a[], int left, int mid, int right){
    n=right-left;
    for(int i=0;i<n-1;i++)
    {
    for(int j=i+1;j<n;j++)
    {
    int t=a[i];
    a[i]=a[j];
    a[j]=t;

    }
    }


}
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
void* msort(void* b) {
    struct Thd* data = (struct Thd*)b;
    int* arr = data->arr;
    int start = data->start;
    int end = data->end;

    if (start < end) {
        int mid = start + (end - start) / 2;

        struct Thd left_data = { arr, start, mid };
        struct Thd right_data = { arr, mid + 1, end };

        pthread_t lthread, rthread;

        pthread_create(&lthread, NULL, msort, &left_data);
        pthread_create(&rthread, NULL, msort, &right_data);

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
struct Thd {
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
void* msort(void* b) {
  pthread_join(lthread, NULL);
        pthread_join(rthread, NULL);

        merge(arr, start, mid, end);
    }

    pthread_exit(NULL);
}
void main()
{
int n;
printf("Enter no.of Elements:");
scanf("%d",&n);
int a[n];
printf("Enter the Elements:");
for(int i=0;i<n;i++)
{
scanf("%d",&a[i]);
}
struct Thd data = {a, 0, n - 1 };
pthread_t mthread;
pthread_create(&mthread, NULL, msort, &data);
pthread_join(mthread, NULL);
 printf("Sorted Elements are: ");
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");

}