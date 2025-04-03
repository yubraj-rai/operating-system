#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 1234  // Same key as the writer process
#define SHM_SIZE 1024  // Memory segment size

int main() {
    // Locate the shared memory segment
    int shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid == -1) {
        std::cerr << "Shared memory access failed\n";
        return 1;
    }

    // Attach to shared memory
    char* shared_memory = (char*)shmat(shmid, nullptr, 0);
    if (shared_memory == (char*)-1) {
        std::cerr << "Memory attachment failed\n";
        return 1;
    }

    // Read and display the shared data
    std::cout << "Data read from shared memory: " << shared_memory << std::endl;

    // Detach and remove shared memory
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, nullptr);  // Remove shared memory segment

    return 0;
}

