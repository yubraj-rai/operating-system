// chat_user.cpp
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include "chat_common.h"

std::atomic<bool> running(true);

void receive_loop(ChatBuffer *recv_buf) {
    while (running) {
        if (recv_buf->message_ready) {
            std::string msg = recv_buf->message;
            std::cout << "\nFriend: " << msg << std::endl;
            recv_buf->message_ready = false;

            if (msg == "exit") {
                running = false;
                break;
            }
        }
        usleep(10000);
    }
}

void send_loop(ChatBuffer *send_buf) {
    std::string input;
    while (running) {
        std::getline(std::cin, input);
        if (input.empty()) continue;

        while (send_buf->message_ready) {
            usleep(10000);
        }

        std::strncpy(send_buf->message, input.c_str(), MSG_SIZE);
        send_buf->message_ready = true;

        if (input == "exit") {
            running = false;
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2 || (std::string(argv[1]) != "1" && std::string(argv[1]) != "2")) {
        std::cerr << "Usage: " << argv[0] << " [1|2] (User 1 or 2)" << std::endl;
        return 1;
    }

    bool is_user1 = std::string(argv[1]) == "1";

    int send_key = is_user1 ? SHM_KEY1 : SHM_KEY2;
    int recv_key = is_user1 ? SHM_KEY2 : SHM_KEY1;

    // Create or get shared memory
    int send_id = shmget(send_key, sizeof(ChatBuffer), IPC_CREAT | 0666);
    int recv_id = shmget(recv_key, sizeof(ChatBuffer), IPC_CREAT | 0666);
    if (send_id == -1 || recv_id == -1) {
        perror("shmget");
        return 1;
    }

    // Attach to shared memory
    auto *send_buf = (ChatBuffer *)shmat(send_id, nullptr, 0);
    auto *recv_buf = (ChatBuffer *)shmat(recv_id, nullptr, 0);
    if (send_buf == (void *)-1 || recv_buf == (void *)-1) {
        perror("shmat");
        return 1;
    }

    std::cout << "Chat started. Type 'exit' to quit.\n";

    std::thread recv_thread(receive_loop, recv_buf);
    send_loop(send_buf);

    recv_thread.join();

    shmdt(send_buf);
    shmdt(recv_buf);

    if (is_user1) {
        shmctl(send_id, IPC_RMID, nullptr);
        shmctl(recv_id, IPC_RMID, nullptr);
    }

    std::cout << "Chat ended.\n";
    return 0;
}

