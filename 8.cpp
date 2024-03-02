#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("/tmp", 'A'); // Generate a unique key for shared memory

    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666); // Create a shared memory segment

    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    char *shmaddr = shmat(shmid, NULL, 0); // Attach to the shared memory segment

    if (shmaddr == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // Write a message to the shared memory
    strcpy(shmaddr, "Hello from process A");

    printf("Process A wrote: %s
", shmaddr);

    // Detach from the shared memory
    shmdt(shmaddr);

    // Attach to the shared memory segment again
    shmaddr = shmat(shmid, NULL, 0);

    if (shmaddr == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    printf("Process B read: %s
", shmaddr);

    // Detach from the shared memory
    shmdt(shmaddr);

    // Destroy the shared memory segment
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

