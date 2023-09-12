// MainNode.h

#ifndef MAINNODE_H
#define MAINNODE_H

#include "../node.h"
#include <vector>

class MainNode {
private:
    uint16_t id;
    std::vector<uint16_t> ids; // Array of 2-byte IDs
    std::vector<int> send_ports;
    std::vector<int> receive_ports;
    std::vector<Node> nodes;

public:
    MainNode(const std::vector<uint16_t>& ids, const std::vector<int>& send_ports, const std::vector<int>& receive_ports, uint16_t id);

    bool initializeAll();
    bool send(uint16_t id, Message& msg);
    Message receive(uint16_t id);
};

#endif // MAINNODE_H
