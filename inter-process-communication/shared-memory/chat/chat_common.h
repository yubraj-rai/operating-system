// chat_common.h
#ifndef CHAT_COMMON_H
#define CHAT_COMMON_H

#include <cstring>
#define SHM_KEY1 1234  // Sender → Receiver
#define SHM_KEY2 5678  // Receiver → Sender
#define MSG_SIZE 256

struct ChatBuffer {
    bool message_ready;
    char message[MSG_SIZE];

    ChatBuffer() : message_ready(false) {
        std::memset(message, 0, MSG_SIZE);
    }
};

#endif

