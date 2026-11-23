#include <stdio.h>

#define max 10
#define max_pages 100
#define max_frames 100

struct page {
 int pagenumber;
    int framenumber;
 int valid;
};

struct process {
    int pid;
 char name[30];
    int memory;
  int pages;
    struct page pt[max_pages];
};

struct frame {
 int pid;
    int pagenumber;
  int occupied;
    int lastused;
};

int main()
{
 struct process p[max];
    struct frame f[max_frames];

 int totalmemory,pagesize,totalframes;
    int n,i,j;
 int pid,pagenumber,processindex;
   int freeframe,usedframes;
 int requests;
    int time = 0;
 int lru,oldpid,oldpage;

 printf("===== memory shadow =====\n");

    printf("enter total physical memory (kb): ");
 scanf("%d",&totalmemory);

 if(totalmemory <= 0)
    {
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

    if(totalframes > max_frames)
 {
 printf("too many frames\n");
      return 0;
 }

 printf("total frames: %d\n",totalframes);

 for(i = 0; i < totalframes; i++)
    {
 f[i].pid = -1;
    f[i].pagenumber = -1;
 f[i].occupied = 0;
      f[i].lastused = 0;
 }

 printf("enter number of processes: ");
 scanf("%d",&n);

    if(n < 1 || n > max) {
 printf("invalid number of processes\n");
   return 0;
 }

 for(i = 0; i < n; i++)
 {
    p[i].pid = i + 1;

 printf("\nenter process %d name: ",i + 1);
    scanf("%29s",p[i].name);

 printf("enter memory required (kb): ");
 scanf("%d",&p[i].memory);

    if(p[i].memory <= 0)
 {
 printf("invalid memory requirement\n");
      return 0;
 }

 p[i].pages = p[i].memory / pagesize;

 if(p[i].memory % pagesize != 0) {
    p[i].pages++;
 }

    if(p[i].pages > max_pages)
 {
 printf("process exceeds maximum supported pages\n");
 return 0;
    }

 printf("pages required: %d\n",p[i].pages);

 for(j = 0; j < p[i].pages; j++)
    {
 p[i].pt[j].pagenumber = j;
      p[i].pt[j].framenumber = -1;
 p[i].pt[j].valid = 0;
 }
 }

 printf("\n===== process details =====\n");
    printf("pid\tname\tmemory\tpages\n");

 for(i = 0; i < n; i++) {
 printf("%d\t%s\t%d kb\t%d\n",
    p[i].pid,p[i].name,p[i].memory,p[i].pages);
 }

 printf("\nenter number of page requests: ");
 scanf("%d",&requests);

 if(requests < 0)
 {
    printf("invalid number of requests\n");
 return 0;
 }

 for(i = 0; i < requests; i++)
    {
 printf("\n===== page request %d =====\n",i + 1);

    printf("enter process id: ");
 scanf("%d",&pid);

 if(pid < 1 || pid > n) {
 printf("invalid process id\n");
    continue;
 }

 processindex = pid - 1;

 printf("enter page number: ");
    scanf("%d",&pagenumber);

 if(pagenumber < 0 || pagenumber >= p[processindex].pages)
 {
 printf("invalid page number\n");
      continue;
 }

 time++;

 if(p[processindex].pt[pagenumber].valid == 1)
    {
 printf("page hit\n");

 freeframe = p[processindex].pt[pagenumber].framenumber;

    f[freeframe].lastused = time;

 printf("page already present in frame %d\n",freeframe);
 }
 else
 {
    printf("page fault\n");

 freeframe = -1;

 for(j = 0; j < totalframes; j++) {
    if(f[j].occupied == 0)
 {
 freeframe = j;
    break;
 }
 }

 if(freeframe == -1)
    {
 lru = 0;

 for(j = 1; j < totalframes; j++)
 {
    if(f[j].lastused < f[lru].lastused) {
 lru = j;
    }
 }

 oldpid = f[lru].pid;
    oldpage = f[lru].pagenumber;

 printf("no free frames available\n");
 printf("lru selected frame %d\n",lru);
    printf("removing page %d of process %d\n",oldpage,oldpid);

 p[oldpid - 1].pt[oldpage].valid = 0;
    p[oldpid - 1].pt[oldpage].framenumber = -1;

 freeframe = lru;
 }

 f[freeframe].pid = pid;
    f[freeframe].pagenumber = pagenumber;
 f[freeframe].occupied = 1;
      f[freeframe].lastused = time;

 p[processindex].pt[pagenumber].framenumber = freeframe;
    p[processindex].pt[pagenumber].valid = 1;

 printf("page %d of process %d loaded into frame %d\n",
    pagenumber,pid,freeframe);
 }
 }

 printf("\n===== page tables =====\n");

 for(i = 0; i < n; i++)
 {
    printf("\nprocess %d (%s)\n",p[i].pid,p[i].name);
 printf("page\tframe\tvalid\n");

 for(j = 0; j < p[i].pages; j++) {
 printf("%d\t%d\t%d\n",
    p[i].pt[j].pagenumber,
 p[i].pt[j].framenumber,
      p[i].pt[j].valid);
 }
 }

 printf("\n===== frame table =====\n");
 printf("frame\tpid\tpage\tstatus\tlastused\n");

 usedframes = 0;

 for(i = 0; i < totalframes; i++)
    {
 printf("%d\t%d\t%d\t",i,f[i].pid,f[i].pagenumber);

 if(f[i].occupied == 0)
 {
    printf("free\t");
 }
 else {
 printf("occupied\t");
    usedframes++;
 }

 printf("%d\n",f[i].lastused);
 }

 printf("\n===== memory summary =====\n");
    printf("total memory: %d kb\n",totalmemory);
 printf("page size: %d kb\n",pagesize);
 printf("total frames: %d\n",totalframes);
    printf("used frames: %d\n",usedframes);
 printf("free frames: %d\n",totalframes - usedframes);
 printf("used memory: %d kb\n",usedframes * pagesize);
    printf("available memory: %d kb\n",(totalframes - usedframes) * pagesize);

 return 0;
}
