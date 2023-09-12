// Node.h

#ifndef NODE_H
#define NODE_H

#include "message.h"
#include <string>

class Node {
private:
    std::string ip;
    int send_port;
    int receive_port;
    int send_socket_fd;
    int receive_socket_fd;
    uint16_t counter;
    uint16_t id; // New 2-byte id field

public:
    Node(const std::string& ip, int send_port, int receive_port, uint16_t id);

    bool initialize();
    bool send(Message& msg);
    Message receive();
    Message processMessage(const Message& msg);
};

#endif // NODE_H
