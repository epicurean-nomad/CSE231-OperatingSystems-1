# Modifying the CFS in Linux 5.9.0

Name: Harsh Kumar

Roll No: 2019043

## Explanation

We have to modify the CFS Scheduler in Linux, to add a soft realtime 
requirement for a given process, identified with it's `pid`.

### Steps for Modifying CFS

1. We add a variable `softruntime` of type `u64` in the `struct sched_entity`, which is defined in the file `include/linux/sched.h`. This is for storing the soft realtime requirement for a given process.

2. To initialize the variable defined in Step 1, we modify the file `kernel/sched/core.c`. We initialize the variable in `__sched_fork` function, where other variable of `struct sched_entity` are initialized.

3. We now modify the function `entity_before` in the file `kernel/sched/fair.c`. Here, we are deciding which process whould be scheduled first. We add the criteria of `softruntime`. If some process has higher `softruntime`, it'll be scheduled first. If there's a tie, it would be resolved using `vruntime`.

4. Now each time the process is preempted by another process, we have to ensure that we reduce the `softrutime` variable. So, in the function `update_curr` in the same file, where `vruntime` is updated after process is preempted, I update `softruntime` too.

5. Next comes the function, `wakeup_preempt_entity` in the same file. We decide if a process have to preempty the currently running process, first on the basis of `softruntime` requirements, then on the basis of `vruntime`.


### Defining Helper Syscall `rtnice`

We now have to define a helper `syscall` in our kernel. We create a new folder so that it's easier to track changes. Also, add the folder to kernel by modifying the kernel Makefile.

In the folder, we create a file `rtnice.c`. We define a syscall named `rtnice` with two parameters, `pid: int` and `softruntime: s64`.

We add the corresponding entry for the syscall in `arch/x86/entry/syscall/syscall_64.tbl`. Also, define the function prototype in `include/linux/syscalls.h`.

We are now ready to compile the kernel.

## Testing the Kernel

We fork two processes. In one process we do a system call and add soft realtime requirements for the process. We note the time. This should ideally run faster. We display the time elapsed in busy wait at the end.

In the other process, we first wait for the other one to complete. Then, we note the time, and then simply do a busy wait. After it completes we display the time elapsed.

Ideally, the one with soft realtime requirements should elapse less time. We observe the same in the output.

## Instructions to Run the Test

```sh
make
````

This will automatically compile and run the program for you. It will ask for `softruntime` value. Enter value in nanoseconds. Press enter.
