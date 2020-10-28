/* Name: Harsh Kumar
   Roll_Number: 2019043 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int var = 10;

void *function() {
   while(var > -90) {
      var--;
      printf("Child: var = %d\n", var);
   }
   printf("Child finished with var = %d\n", var);
}

int main() {
   pthread_t tid;
   pthread_create(&tid, NULL, function, NULL);

   while(var < 100) {
      var++;
      printf("Parent: var = %d\n", var);
   }
   printf("Parent finished with var = %d\n", var);
   pthread_join(tid, NULL);
}