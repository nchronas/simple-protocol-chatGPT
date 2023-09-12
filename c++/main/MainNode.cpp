// MainNode.cpp

#include "MainNode.h"

MainNode::MainNode(const std::vector<uint16_t>& ids, const std::vector<int>& send_ports, const std::vector<int>& receive_ports, uint16_t id)
    : ids(ids), send_ports(send_ports), receive_ports(receive_ports), id(id) {
    for (size_t i = 0; i < ids.size(); ++i) {
        nodes.emplace_back("127.0.0.1", send_ports[i], receive_ports[i], ids[i]);
    }
}

bool MainNode::initializeAll() {
    for (auto& node : nodes) {
        if (!node.initialize()) {
            return false;
        }
    }
    return true;
}

bool MainNode::send(uint16_t id, Message& msg) {
    for (size_t i = 0; i < ids.size(); ++i) {
        if (ids[i] == id) {

            // Set source ID of the message to the ID of the Node
            msg.setSourceID(this->id);
            return nodes[i].send(msg);
        }
    }
    return false; // ID not found
}

Message MainNode::receive(uint16_t id) {
    for (size_t i = 0; i < ids.size(); ++i) {
        if (ids[i] == id) {
            return nodes[i].receive();
        }
    }
    return Message(0, 0, 0, 0, 0, {}); // ID not found, return an empty message
}
