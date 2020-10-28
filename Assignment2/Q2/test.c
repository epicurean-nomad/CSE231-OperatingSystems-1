/* Name: Harsh Kumar
   Roll_Number: 2019043 */
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define __NR_identity 440

long sh_task_info(int pid, char *filename) {
   return syscall(__NR_identity, pid, filename);
}

int main(int argc, char *argv[]){
   int pid;
   char path[200];

   printf("Enter pid: ");
   scanf("%d", &pid);
   printf("Enter flepath: ");
   scanf("%s", path);

   if(sh_task_info(pid, path) < 0){
      perror("System call failed");
   }
   else{
      printf("System call successful! Check kernel log.\n");
   }

   return 0;
}