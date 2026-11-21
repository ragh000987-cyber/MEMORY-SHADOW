#include <stdio.h>

#define max 10
#define max_pages 100
#define max_frames 100

struct pagetable {
    int pagenumber;
    int framenumber;
    int valid;
};

struct process {
    int pid;
    char name[30];
    int memory;
    int allocated;
    int pages;
    struct pagetable pt[max_pages];
};

struct frame {
    int pid;
    int pagenumber;
    int occupied;
};

int main() {
    struct process p[max];
    struct frame f[max_frames];

    int totalmemory,availablememory;
    int pagesize,totalframes;
    int n,i,j;

    printf("===== memory shadow =====\n");

    printf("enter total physical memory (kb): ");
    scanf("%d",&totalmemory);

    if(totalmemory <= 0) {
        printf("invalid memory size\n");
        return 0;
    }

    printf("enter page size (kb): ");
    scanf("%d",&pagesize);

    if(pagesize <= 0 || pagesize > totalmemory) {
        printf("invalid page size\n");
        return 0;
    }

    totalframes = totalmemory / pagesize;

    if(totalframes > max_frames) {
        printf("too many frames\n");
        return 0;
    }

    printf("total frames: %d\n",totalframes);

    for(i = 0; i < totalframes; i++) {
        f[i].pid = -1;
        f[i].pagenumber = -1;
        f[i].occupied = 0;
    }

    availablememory = totalmemory;

    printf("enter number of processes: ");
    scanf("%d",&n);

    if(n < 1 || n > max) {
        printf("invalid number of processes\n");
        return 0;
    }

    for(i = 0; i < n; i++) {

        p[i].pid = i + 1;
        p[i].allocated = 0;

        printf("\nenter process %d name: ",i + 1);
        scanf("%29s",p[i].name);

        printf("enter memory required (kb): ");
        scanf("%d",&p[i].memory);

        if(p[i].memory <= 0) {
            printf("invalid memory requirement\n");
            return 0;
        }

        p[i].pages = p[i].memory / pagesize;

        if(p[i].memory % pagesize != 0) {
            p[i].pages++;
        }

        if(p[i].pages > max_pages) {
            printf("process exceeds maximum supported pages\n");
            return 0;
        }

        printf("pages required: %d\n",p[i].pages);

        for(j = 0; j < p[i].pages; j++) {
            p[i].pt[j].pagenumber = j;
            p[i].pt[j].framenumber = -1;
            p[i].pt[j].valid = 0;
        }

        if(p[i].memory <= availablememory) {
            p[i].allocated = 1;
            availablememory -= p[i].memory;

            printf("memory allocated successfully\n");
        }
        else {
            printf("memory allocation failed\n");
        }
    }

    printf("\n===== process details =====\n");

    printf("pid\tname\tmemory\tpages\tstatus\n");

    for(i = 0; i < n; i++) {

        printf("%d\t%s\t%d kb\t%d\t",
        p[i].pid,p[i].name,p[i].memory,p[i].pages);

        if(p[i].allocated == 1) {
            printf("allocated\n");
        }
        else {
            printf("failed\n");
        }
    }

    printf("\n===== frame table =====\n");

    printf("frame\tpid\tpage\tstatus\n");

    for(i = 0; i < totalframes; i++) {

        printf("%d\t%d\t%d\t",i,f[i].pid,f[i].pagenumber);

        if(f[i].occupied == 0) {
            printf("free\n");
        }
        else {
            printf("occupied\n");
        }
    }

    printf("\ntotal memory: %d kb\n",totalmemory);
    printf("total frames: %d\n",totalframes);
    printf("page size: %d kb\n",pagesize);
    printf("used memory: %d kb\n",totalmemory - availablememory);
    printf("available memory: %d kb\n",availablememory);

    return 0;
}
