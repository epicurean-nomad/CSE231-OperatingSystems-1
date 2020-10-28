/* Name: Harsh Kumar
   Roll_Number: 2019043 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int var = 10;

int main() {
   int pid;
   pid = fork();

   if(pid < 0) {
      perror("Forking failed");
      return -1;
   }
   else if(pid == 0) {
      // Child process
      while(var > -90) {
         var--;
         printf("Child: var = %d\n", var);
      }
      
      printf("Child finished with var = %d\n", var);
      exit(0);
   }
   else {
      // Parent Process
      while(var < 100) {
         var++;
         printf("Parent: var = %d\n", var);
      }
      printf("Parent finished with var = %d\n", var);
      wait(NULL);
   }
}