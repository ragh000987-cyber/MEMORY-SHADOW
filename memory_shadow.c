#include <stdio.h>

#define MAX 10
#define MAX_PAGES 100

struct PageTable {
    int pageNumber;
    int frameNumber;
    int valid;
};

struct Process {
    int pid;
    char name[30];
    int memory;
    int allocated;
    int pages;
    struct PageTable pageTable[MAX_PAGES];
};

int main()
{ struct Process p[MAX];
 int totalMemory, availableMemory,pagesize,totalframes;
    int n, i;
    printf("===== MEMORY SHADOW =====\n");
   printf("Enter total physical memory (KB): ");
  scanf("%d", &totalMemory);

if (totalMemory <= 0) {
        printf("Invalid memory size\n");
        return 0;
    }

  printf("Enter page size (KB): ");
scanf("%d", &pageSize);

if (pageSize <= 0 || pageSize > totalMemory) {
    printf("Invalid page size\n");
    return 0;
}

totalFrames = totalMemory / pageSize;

printf("Total Frames: %d\n", totalFrames);
 availableMemory = totalMemory;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    if (n < 1 || n > MAX) {
        printf("Invalid number of processes\n");
        return 0;
    }

    for (i = 0; i < n; i++) {

        p[i].pid = i + 1;
        p[i].allocated = 0;

        printf("\nEnter Process %d name: ", i + 1);
        scanf("%29s", p[i].name);

        printf("Enter memory required (KB): ");
        scanf("%d", &p[i].memory);

        if (p[i].memory <= 0) {
            printf("Invalid memory requirement\n");
            return 0;
        }

        if (p[i].memory <= availableMemory) {
            p[i].allocated = 1;
            availableMemory -= p[i].memory;

            printf("Memory allocated successfully\n");
        } else {
            printf("Memory allocation failed\n");
        }
    }

    printf("\n PROCESS DETAILS \n");

    printf("PID\tNAME\tMEMORY\tSTATUS\n");

    for (i = 0; i < n; i++) {

        printf("%d\t%s\t%d KB\t",
               p[i].pid,
               p[i].name,
               p[i].memory);

        if (p[i].allocated == 1) {
            printf("Allocated\n");
        } else {
            printf("Failed\n");
        }
    }

    printf("\nTotal Memory: %d KB\n", totalMemory);
    printf("Used Memory: %d KB\n", totalMemory - availableMemory);
    printf("Available Memory: %d KB\n", availableMemory);

    return 0;
}
