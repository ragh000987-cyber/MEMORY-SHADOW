#include <stdio.h>

#define MAX 10
#define MAX_PAGES 100

struct PageTable {
    int pagenumber;
    int framenumber;
    int valid;
};

struct Process {
    int pid;
    char name[30];
    int memory;
    int allocated;
    int pages;
    struct PageTable pagetable[MAX_PAGES];
};

int main() {
    struct Process p[MAX];

    int totalmemory,availablememory;
    int pagesize,totalframes;
    int n,i,j;

    printf("===== MEMORY SHADOW =====\n");

    printf("Enter total physical memory (KB): ");
    if (scanf("%d",&totalmemory) != 1 || totalmemory <= 0) {
        printf("Invalid memory size\n");
        return 0;
    }

    printf("Enter page size (KB): ");
    if (scanf("%d",&pagesize) != 1 ||
        pagesize <= 0 || pagesize > totalmemory) {
        printf("Invalid page size\n");
        return 0;
    }

    totalframes = totalmemory / pagesize;

    printf("Total Frames: %d\n",totalframes);

    availablememory = totalmemory;

    printf("Enter number of processes: ");
    if (scanf("%d",&n) != 1 || n < 1 || n > MAX) {
        printf("Invalid number of processes\n");
        return 0;
    }

    for (i = 0; i < n; i++) {

        p[i].pid = i + 1;
        p[i].allocated = 0;

        printf("\nEnter Process %d name: ",i + 1);
        if (scanf("%29s",p[i].name) != 1) {
            printf("Invalid process name\n");
            return 0;
        }

        printf("Enter memory required (KB): ");
        if (scanf("%d",&p[i].memory) != 1 || p[i].memory <= 0) {
            printf("Invalid memory requirement\n");
            return 0;
        }

        p[i].pages = p[i].memory / pagesize;

        if (p[i].memory % pagesize != 0) {
            p[i].pages++;
        }

        if (p[i].pages > MAX_PAGES) {
            printf("Process exceeds maximum supported pages\n");
            return 0;
        }

        printf("Pages required: %d\n",p[i].pages);

        for (j = 0; j < p[i].pages; j++) {
            p[i].pagetable[j].pagenumber = j;
            p[i].pagetable[j].framenumber = -1;
            p[i].pagetable[j].valid = 0;
        }

        if (p[i].memory <= availablememory) {
            p[i].allocated = 1;
            availablememory -= p[i].memory;

            printf("Memory allocated successfully\n");
        } else {
            printf("Memory allocation failed\n");
        }
    }

    printf("\n===== PROCESS DETAILS =====\n");

    printf("PID\tNAME\tMEMORY\tPAGES\tSTATUS\n");

    for (i = 0; i < n; i++) {

        printf("%d\t%s\t%d KB\t%d\t",
               p[i].pid,
               p[i].name,
               p[i].memory,
               p[i].pages);

        if (p[i].allocated == 1) {
            printf("Allocated\n");
        } else {
            printf("Failed\n");
        }
    }

    printf("\nTotal Memory: %d KB\n",totalmemory);
    printf("Total Frames: %d\n",totalframes);
    printf("Page Size: %d KB\n",pagesize);
    printf("Used Memory: %d KB\n",totalmemory - availablememory);
    printf("Available Memory: %d KB\n",availablememory);

    return 0;
}
