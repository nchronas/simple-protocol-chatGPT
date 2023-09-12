#include "../node.h"
#include <iostream>

int main() {
    std::cout << "Hello" << std::endl;

    Node node("127.0.0.1", 1025, 1026, 10);

    if (!node.initialize()) {
        return 1;
    }

    while (true) {
        Message received_msg = node.receive();

        if (received_msg.isEmpty()) {
            std::cout << "Received empty Message" << std::endl;
            continue;
        }

        std::cout << "Received Message:" << std::endl;
        std::cout << "Source ID: " << received_msg.getSourceID() << std::endl;
        std::cout << "Destination ID: " << received_msg.getDestinationID() << std::endl;
        std::cout << "Counter: " << received_msg.getCounter() << std::endl;
        std::cout << "Index: " << (int)received_msg.getIndex() << std::endl;
        std::cout << "Sub Index: " << (int)received_msg.getSubIndex() << std::endl;

        if (received_msg.getData().size() > 0) {
            std::cout << "Data: ";
            for (uint8_t byte : received_msg.getData()) {
                std::cout << std::hex << (int)byte << " ";
            }
            std::cout << std::endl;
        }
        else {
            std::cout << "No data." << std::endl;
        }

        Message response_msg = node.processMessage(received_msg);
        if (!response_msg.isEmpty()) {
            node.send(response_msg); // Assuming you want to send to localhost port 1026
            std::cout << "Sent response." << std::endl;
        }
        else {
            std::cout << "No response sent." << std::endl;
        }
    }

    return 0;
}
