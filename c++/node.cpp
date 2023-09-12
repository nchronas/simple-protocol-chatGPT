// Node.cpp

#include "node.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

Node::Node(const std::string& ip, int send_port, int receive_port, uint16_t id) : ip(ip), send_port(send_port), receive_port(receive_port), send_socket_fd(-1), receive_socket_fd(-1), counter(0), id(id) {}

bool Node::initialize() {
    send_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    receive_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (send_socket_fd < 0 || receive_socket_fd < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return false;
    }

    sockaddr_in send_addr;
    memset(&send_addr, 0, sizeof(send_addr));
    send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons(send_port);
    send_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // if (bind(send_socket_fd, (struct sockaddr*)&send_addr, sizeof(send_addr)) < 0) {
    //     std::cerr << "Failed to bind sending socket." << std::endl;
    //     return false;
    // }

    sockaddr_in receive_addr;
    memset(&receive_addr, 0, sizeof(receive_addr));
    receive_addr.sin_family = AF_INET;
    receive_addr.sin_port = htons(receive_port);
    receive_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(receive_socket_fd, (struct sockaddr*)&receive_addr, sizeof(receive_addr)) < 0) {
        std::cerr << "Failed to bind receiving socket." << std::endl;
        return false;
    }

    return true;
}

bool Node::send(Message& msg) {
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(send_port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    // Increase the counter and put it into the message
    msg.setCounter(++counter);

    std::vector<uint8_t> encoded_msg = msg.encode();

    ssize_t sent_bytes = sendto(send_socket_fd, encoded_msg.data(), encoded_msg.size(), 0, (struct sockaddr*)&addr, sizeof(addr));

    if (sent_bytes < 0) {
        std::cerr << "Failed to send message." << std::endl;
        return false;
    }

    return true;
}

Message Node::receive() {
    std::vector<uint8_t> buffer(1024); // Adjust the buffer size as needed

    sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    ssize_t received_bytes = recvfrom(receive_socket_fd, buffer.data(), buffer.size(), 0, (struct sockaddr*)&addr, &addr_len);

    if (received_bytes < 0) {
        std::cerr << "Failed to receive message." << std::endl;
        return Message(0, 0, 0, 0, 0, {});
    }

    std::vector<uint8_t> received_data(buffer.begin(), buffer.begin() + received_bytes);

    return Message::decode(received_data);
}

Message Node::processMessage(const Message& msg) {
    if (msg.getIndex() == 17 && msg.getSubIndex() == 1) {
        Message newMsg(msg.getSourceID(), msg.getDestinationID(), 0, 17, 2, {});
        return newMsg;
    }
    else {
        return Message(0, 0, 0, 0, 0, {});
    }
}