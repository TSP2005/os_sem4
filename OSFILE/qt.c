#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESS 100

typedef struct {
    char name[20]; 
    int arrival;
    int cpuburst;
    int turnaround;
    int wait;
} Process;

Process processtable[MAX_PROCESS];
Process processtablecopy[MAX_PROCESS];
int current_time;
int n;

void ReadProcessTable(char *filename);
void PrintProcessTable();
void FCFS();
void RR(int quantum);
void SRBF();
void PrintStatistics();

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_filename>\n", argv[0]);
        return 1;
    }
    ReadProcessTable(argv[1]);
    PrintProcessTable(); 
    int choice;
    int quantum;
    do {
        printf("\n-------------------------------------------------\n");
        printf("CPU Scheduling Simulation\n");
        printf("-------------------------------------------------\n");
        printf("Select the scheduling algorithm [1,2,3 or 4]:\n");
        printf("1. First Come First Served (FCFS)\n");
        printf("2. Round Robin (RR)\n");
        printf("3. SRBF\n");
        printf("4. Exit\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                FCFS();
                PrintStatistics();
                break;
            case 2:
                printf("Enter the time quantum: ");
                scanf("%d", &quantum);
                RR(quantum);
                PrintStatistics();
                break;
            case 3:
                SRBF();
                PrintStatistics();
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please select again.\n");
                break;
        }
    } while (choice != 4);

    return 0;
}

void ReadProcessTable(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    n = 0;
    while (fscanf(file, "%s %d %d", processtablecopy[n].name, &processtablecopy[n].arrival, &processtablecopy[n].cpuburst) != EOF) {
        n++;
    }
    fclose(file);
}

void PrintProcessTable() {
    printf("Process Table:\n");
    printf("Name\tArrival\tCPUBurst\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\n", processtablecopy[i].name, processtablecopy[i].arrival, processtablecopy[i].cpuburst);
    }
}

void FCFS() {
    for(int i=0;i<n;i++)
    {
    processtable[i]=processtablecopy[i];
    }
    current_time = 0;
    for (int i = 0; i < n; i++) {
        int c=current_time;
        if (processtable[i].arrival > current_time) {
            current_time = processtable[i].arrival;
        }
        processtable[i].turnaround = current_time + processtable[i].cpuburst - processtable[i].arrival;
        processtable[i].wait = processtable[i].turnaround - processtable[i].cpuburst;
        current_time += processtable[i].cpuburst;
        printf("[%d-%d]\t%s\trunning \n",c,current_time,processtable[i].name);
    }
}

void RR(int quantum) {
    for(int i = 0; i < n; i++) {
        processtable[i] = processtablecopy[i];
    }
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processtable[j].arrival > processtable[j + 1].arrival) {
                Process temp = processtable[j];
                processtable[j] = processtable[j + 1];
                processtable[j + 1] = temp;
            }
        }
    }
    int p[n];
    current_time = 0;
    for (int i = 0; i < n; i++) {
        p[i] = processtable[i].cpuburst;
    }
    int s = 0;
    while (s < n) {
        for (int i = 0; i < n; i++) {
            if (processtable[i].arrival > current_time) {
                continue;
            } else {
                if (p[i] > 0) {
                    if (p[i] > quantum) {
                        printf("[%d-%d]\t%s\trunning\n", current_time, current_time + quantum, processtable[i].name);
                        current_time += quantum;
                        p[i] -= quantum;
                    } else {
                        printf("[%d-%d]\t%s\trunning\n", current_time, current_time + p[i], processtable[i].name);
                        current_time += p[i];
                        processtable[i].turnaround = current_time - processtable[i].arrival;
                        processtable[i].wait = current_time - processtable[i].arrival - processtable[i].cpuburst;
                        p[i] = 0;
                        s++;
                    }
                }
            }
        }
    }
}

void SRBF() {
     for(int i=0;i<n;i++)
    {
    processtable[i]=processtablecopy[i];
    }
     for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processtable[j].cpuburst > processtable[j + 1].cpuburst) {
                Process temp = processtable[j];
                processtable[j] = processtable[j + 1];
                processtable[j + 1] = temp;
            }
        }
    }
    current_time = 0;
    int k=0;
    int p[sizeof(processtable)/sizeof(processtable[0])];
    for(int i=0;i<n;i++)
    {
    p[i]=processtable[i].cpuburst;
    }
    int s;
    while (k < n)
    {
        s = -1;
        for (int i = 0; i < n; i++)
        {
        if (processtable[i].arrival > current_time)
        {
        continue;   
        }
        if (p[i] > 0 && (s < 0 || p[i] < p[s]))
        {
        s = i;
        }
        }
        printf("[%d-%d]\t%s\trunning \n", current_time, current_time + 1, processtable[s].name);
         for (int i = 0; i < n; i++)
        {
            if ((p[i] > 0) && (i != s))
            {
                processtable[i].wait += 1;
            }
        }
       
        p[s] -= 1;
        current_time+=1;
        if (p[s] == 0)
        {
            k++;
            processtable[s].turnaround = current_time - processtable[s].arrival;
        }
    }
    for (int i = 0; i < n; i++)
    {
        processtable[i].wait -= processtable[i].arrival;
    }
}

void PrintStatistics() {
    printf("\nTurnaround times: ");
    for (int i = 0; i < n; i++) {
        printf("%s[%d] ", processtable[i].name, processtable[i].turnaround);
    }
    printf("\nWait times: ");
    for (int i = 0; i < n; i++) {
        printf("%s[%d] ", processtable[i].name, processtable[i].wait);
    }
    printf("\nAverage turnaround time: ");
    float avg_turnaround = 0;
    for (int i = 0; i < n; i++) {
        avg_turnaround += processtable[i].turnaround;
    }
    avg_turnaround /= n;
    printf("%f\n", avg_turnaround);
    printf("Average wait time: ");
    float avg_wait = 0;
    for (int i = 0; i < n; i++) {
        avg_wait += processtable[i].wait;
    }
    avg_wait /= n;
    printf("%f\n", avg_wait);
}
