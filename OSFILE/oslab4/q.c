#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_PROCESS 10

typedef struct {
    char * name;
    int arrival;
    int cpuburst;
    int turnaround;
    int wait;
} Process;

Process processtable[MAX_PROCESS];
int current_time = 0;
int process_count = 0;

void ReadProcessTable(char * filename);
void PrintProcessTable();
void FCFS();
void RR(int quantum);
void SRBF();
void PrintStatistics();

int main(int arg_count, char * args[])
{
    if (arg_count != 2)
    {
        printf("Usage: %s <filename>\n", args[0]);
        return 1;
    }
    ReadProcessTable(args[1]);   
    PrintProcessTable();
    int choice;
    while (1)
    {
        char newline;
        printf("-------------------------------------------------\n");
        printf("CPU Scheduling Simulation\n");
        printf("-------------------------------------------------\n");
        printf("Select the scheduling algorithm [1,2,3 or 4]:\n");
        printf("1. First Come First Served (FCFS)\n");
        printf("2. Round Robin (RR)\n");
        printf("3. SRBF\n");
        printf("4. Exit\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1: 
            printf("-------------------------------------------------\n");
            printf("First Come First Served Scheduling\n");
            printf("-------------------------------------------------\n");
            FCFS();
            PrintStatistics();
            current_time = 0;
            break;
        case 2:
            printf("-------------------------------------------------\n");
            printf("Round Robin Scheduling\n");
            printf("-------------------------------------------------\n");
            int quantum;
            printf("Enter the time quantum: ");
            scanf("%d", &quantum);
            RR(quantum);
            PrintStatistics();
            current_time = 0;
            break;
        case 3:
            printf("-------------------------------------------------\n");
            printf("Shortest Remaining Burst First Scheduling\n");
            printf("-------------------------------------------------\n");
            SRBF();
            PrintStatistics();
            current_time = 0;
            break;
        case 4:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Enter a valid option\n");
            printf("Exiting...\n");
            exit(0);
        }
    }    
}

void ReadProcessTable(char * filename)
{
    FILE * file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("ERROR Opening File");
        exit(EXIT_FAILURE);
    }
    char temp_name[10];
    while (fscanf(file, "%s %d %d", temp_name, &processtable[process_count].arrival, &processtable[process_count].cpuburst) == 3)
    {
        processtable[process_count].name = malloc(strlen(temp_name) + 1);
        if (processtable[process_count].name == NULL)
        {
            perror("ERROR Allocating Memory");
            exit(EXIT_FAILURE);
        }
        strcpy(processtable[process_count].name, temp_name);
        processtable[process_count].turnaround = 0;
        processtable[process_count].wait = 0;
        process_count += 1;
        if (process_count >= MAX_PROCESS)
        {
            fprintf(stderr, "Exceeded maximum number of processes\n");
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);
}

void PrintProcessTable()
{
    printf("PROCESS TABLE:\n");
    for (int i = 0; i < process_count; i++)
    {
        printf("Name: %s   Arrival: %d   CPU Burst: %d\n", processtable[i].name, processtable[i].arrival, processtable[i].cpuburst);
    }
}

void FCFS()
{
    for (int i = 0; i < process_count - 1; i++)
    {
        for (int j = 0; j < process_count - i - 1; j++)
        {
            if (processtable[j].arrival > processtable[j + 1].arrival)
            {
                Process temp = processtable[j];
                processtable[j] = processtable[j + 1];
                processtable[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < process_count; i++)
    {
        processtable[i].wait = current_time - processtable[i].arrival;
        processtable[i].turnaround = processtable[i].wait + processtable[i].cpuburst;
        printf("P%d running [%d-%d]\n", i, current_time, current_time + processtable[i].cpuburst);
        current_time += processtable[i].cpuburst;
    }
}

void RR(int quantum)
{
    for (int i = 0; i < process_count - 1; i++)
    {
        for (int j = 0; j < process_count - i - 1; j++)
        {
            if (processtable[j].arrival > processtable[j + 1].arrival)
            {
                Process temp = processtable[j];
                processtable[j] = processtable[j + 1];
                processtable[j + 1] = temp;
            }
        }
    }
    int remaining_burst[process_count];
    for (int i = 0; i < process_count; i++)
    {
        remaining_burst[i] = processtable[i].cpuburst;
    }
    int completed_processes = 0;
    while (completed_processes < process_count)
    {
        for (int i = 0; i < process_count; i++)
        {
            if (processtable[i].arrival > current_time)
            {
                continue;
            }
            else
            {
                if (remaining_burst[i] > 0)
                {
                    if (remaining_burst[i] > quantum)
                    {
                        printf("%s running [%d-%d]\n",processtable[i].name, current_time, current_time + quantum);
                        for (int j = 0; j < process_count; j++)
                        {
                            if (i == j)
                            {
                                continue;
                            }
                            else
                            {
                                if (remaining_burst[j] > 0)
                                {
                                    processtable[j].wait += quantum;
                                }
                            }
                        }
                        current_time += quantum;
                        remaining_burst[i] -= quantum;
                    }
                    else
                    {
                        printf("%s running [%d-%d]\n",processtable[i].name, current_time, current_time + remaining_burst[i]);
                        for (int j = 0; j < process_count; j++)
                        {
                            if (i == j)
                            {
                                continue;
                            }
                            else
                            {
                                if (remaining_burst[j] > 0)
                                {
                                    processtable[j].wait += remaining_burst[i];
                                }
                            }
                        }
                        current_time += remaining_burst[i];
                        processtable[i].turnaround = current_time - processtable[i].arrival;
                        processtable[i].wait = current_time - processtable[i].arrival - processtable[i].cpuburst;
                        remaining_burst[i] = 0;
                    }
                    if (remaining_burst[i] == 0)
                    {
                        completed_processes++;
                    }
                }
            }
        }
    }
}

void SRBF()
{
    for (int i = 0; i < process_count - 1; i++)
    {
        for (int j = 0; j < process_count - i - 1; j++)
        {
            if (processtable[j].arrival > processtable[j + 1].arrival)
            {
                Process temp = processtable[j];
                processtable[j] = processtable[j + 1];
                processtable[j + 1] = temp;
            }
        }
    }
    int remaining_burst[process_count];
    for (int i = 0; i < process_count; i++)
    {
        remaining_burst[i] = processtable[i].cpuburst;
    }
    int completed_processes = 0;
    int shortest_remaining_burst_index;
    while (completed_processes < process_count)
    {
        shortest_remaining_burst_index = -1;
        for (int i = 0; i < process_count; i++)
        {
            if (processtable[i].arrival > current_time)
            {
                continue;   
            }
            else
            {
                if (remaining_burst[i] > 0 && (shortest_remaining_burst_index == -1 || remaining_burst[i] < remaining_burst[shortest_remaining_burst_index]))
                {
                    shortest_remaining_burst_index = i;
                }
            }
        }
        printf("%s running [%d-%d]\n", processtable[shortest_remaining_burst_index].name, current_time, current_time + 1);
        current_time += 1;
        for (int i = 0; i < process_count; i++)
        {
            if ((remaining_burst[i] > 0) && (i != shortest_remaining_burst_index))
            {
                processtable[i].wait += 1;
            }
        }
        remaining_burst[shortest_remaining_burst_index] -= 1;

        if (remaining_burst[shortest_remaining_burst_index] == 0)
        {
            completed_processes++;
            processtable[shortest_remaining_burst_index].turnaround = current_time - processtable[shortest_remaining_burst_index].arrival;
        }
    }
    for (int i = 0; i < process_count; i++)
    {
        processtable[i].wait -= processtable[i].arrival;
    }
}

void PrintStatistics()
{
    printf("Turnaround times:  ");
    for (int i = 0; i < process_count; i++)
    {
        printf("%s[%d]  ", processtable[i].name, processtable[i].turnaround);
    }
    printf("\n");
    printf("Wait times:        ");
    for (int i = 0; i < process_count; i++)
    {
        printf("%s[%d]  ", processtable[i].name, processtable[i].wait);
    }
    printf("\n");
    int total_turnaround = 0;
    for (int i = 0; i < process_count; i++)
    {
        total_turnaround += processtable[i].turnaround;
    }
    float avg_turnaround = total_turnaround / (float)process_count;
    printf("Average turnaround time: %.2f", avg_turnaround);
    printf("\n");
    int total_wait = 0;
    for (int i = 0; i < process_count; i++)
    {
        total_wait += processtable[i].wait;
    }
    float avg_wait = total_wait / (float)process_count;
    printf("Average wait time: %.2f", avg_wait);
    printf("\n");
}