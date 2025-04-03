#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <unistd.h>

#define SHM_KEY 1234  // Unique key for shared memory
#define SHM_SIZE 1024  // Memory segment size

int main() {
    // Create shared memory segment
    int shmid = shmget(SHM_KEY, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        std::cerr << "Shared memory creation failed\n";
        return 1;
    }

    // Attach shared memory to process address space
    char* shared_memory = (char*)shmat(shmid, nullptr, 0);
    if (shared_memory == (char*)-1) {
        std::cerr << "Memory attachment failed\n";
        return 1;
    }

    // Write data to shared memory
    std::string message = "Hello from Process 1!";
    strncpy(shared_memory, message.c_str(), SHM_SIZE);

    std::cout << "Data written to shared memory: " << message << std::endl;

    // Detach from shared memory
    shmdt(shared_memory);

    return 0;
}

