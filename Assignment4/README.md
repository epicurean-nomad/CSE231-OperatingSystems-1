# Assignment 4

Modified Dining Philosopher problem with self defined semaphore using pthread_mutex.

## Makefile instructions

Run the `make` command from terminal form this directory to compile and run.

The program runs infinitely. Send interrupt to close the program.

## Technique Used for making custom Semaphore

`semaphore struct` has four fields : `value:int`, `pending:int`, `lock:pthread_mutex_t` and `cond:pthread_cond_t`. `value` is used for storing the semaphore value (no of available resourcees). `pending` is stored for indicating if there's any room for waiting threads to take a lock on the semaphore. `lock` is used for taking a lock before changing semaphore values. `cond` is a conditional lock, on which waiting threads take lock, if the resource is currently unavailable.

## Technique Used for Deadlock avoidance

Each philosopher need two chopsticks, each adjacent to the position where the philosohper is sitting. He/she also needs one bowl (out of max 2 available) to start eating.

The philosophers pick a chopstick on their left, then the chopstick on their right, then take a bowl and finally start eating. To avoid any deadlock, the philosopher puts back the first chopstick, if he is unable to get the chopstick and tries to eat later. Similarly, if he/she fails to pick the bowl, then they put back both the chopsticks that they have picked earlier.

This way deadlock is avoided, and all of them finish eating.
