#include "MainNode.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    std::vector<uint16_t> ids = {10, 11, 12}; // Example IDs
    std::vector<int> send_ports = {1026, 4026, 5026}; // Example send ports
    std::vector<int> receive_ports = {1025, 4025, 5025}; // Example receive ports
    uint16_t main_id = 17;

    std::cout << "Hello" << std::endl;


    MainNode mainNode(ids, send_ports, receive_ports, main_id);

    if (!mainNode.initializeAll()) {
        std::cerr << "Failed to initialize nodes." << std::endl;
        return 1;
    }

    int iter_id = 10;
    while (true) {

        Message msg(main_id, iter_id, 0, 17, 1, {});
        bool err = mainNode.send(iter_id, msg);
        if(!err) {
            std::cerr << "Failed to find ID" << std::endl;
        }


        // Example usage: Receive a message from node with ID 0x5678
        Message received_msg = mainNode.receive(iter_id);

        if (!received_msg.isEmpty()) {

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
        }

        iter_id++;
        if(iter_id > 12) {
            iter_id = 10;
        }

        std::cout << "Next iteration" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for 60 seconds before next iteration
    }

    return 0;
}
