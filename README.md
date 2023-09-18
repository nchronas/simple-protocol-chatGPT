# simple-protocol-chatGPT

<strong>TL/DR</strong> Creating a small protocol with ChatGPT for multiple languages

## introduction

This time I will explore protocol based code generation for multiple languages using ChatGPT, something that I’m really fond of and enjoyed doing in the past.

This time I will use C++, Python & rust. I will also make an attempt to use Zig a language that I'm not familiar at all.

## Coding challenge

I will make a typical protocol with the a 2 byte source, destination field, a ID/counter that increases each time a node sends a packet, a byte index and subindex field and finally a variable data field max 10 bytes. The index and subindex is a common way like OPENCAN or ECSS to use a table and describe the data field.  I will then create different nodes in different languages, each node having a unique id and using a main node to multiple secondary nodes. The nodes will be connected to the main node and it will route the messages accordingly.

The underlying method of transferring the data will be UDP. Each node will connect to the main node using different ports and the main node will route the messages based on the unique ID.Now there are better ways to do this sort of communication, like MQTT etc but this is not meant to be a production code or efficient way to communicate but more of an exploration of the code generating capabilities of AI and ChatGPT.

### Functionality

I’m going to create a small number of nodes that when they receive a message with index 17 and sub index 1, they will respond with a message with index 17 and sub index 2. For now it won’t be any data.

It will be a main node that sends the message every x seconds and verify that the message was received.

This services is used to simulate a heartbeat, a way to make sure that all nodes are up and running.

## During the "development"

This 2nd attempt was more frustrating but the frustration came mostly from  poor planning in what I'm trying to do, I simply I didn't think it through enough. And that no matter how good an AI is, it cannot do much for poor design.

While I tried to make no code changes, sometimes it was much quicker to do some manual changes than to instruct chatGPT to do it for yourself. This come mostly I think by the complexity of having mutliple classes and applications in one chat.

Generating the code for python was a breeze and very fast. I only had to make the changes I made for the C++ classes. Rust was a bit more difficult but I attribute that to my unfamiliarity with the language.

### C++

#### Message class

The first prompt actually provided a very good start. The next steps were mostly nitpicking like renaming etc.

``` cpp
#include <iostream>
#include <vector>

class Message {
private:
    uint16_t source_id;
    uint16_t destination_id;
    uint16_t counter;
    uint8_t index;
    uint8_t sub_index;
    std::vector<uint8_t> data;

public:
    Message(uint16_t src_id, uint16_t dest_id, uint16_t cnt, uint8_t idx, uint8_t sub_idx, const std::vector<uint8_t>& dt) :
        source_id(src_id),
        destination_id(dest_id),
        counter(cnt),
        index(idx),
        sub_index(sub_idx),
        data(dt) {}

    std::vector<uint8_t> encode() const {
        std::vector<uint8_t> encoded_message;

        // Encode source id (2 bytes)
        encoded_message.push_back((source_id >> 8) & 0xFF);
        encoded_message.push_back(source_id & 0xFF);

        // Encode destination id (2 bytes)
        encoded_message.push_back((destination_id >> 8) & 0xFF);
        encoded_message.push_back(destination_id & 0xFF);

        // Encode counter (2 bytes)
        encoded_message.push_back((counter >> 8) & 0xFF);
        encoded_message.push_back(counter & 0xFF);

        // Encode index (1 byte)
        encoded_message.push_back(index);

        // Encode sub index (1 byte)
        encoded_message.push_back(sub_index);

        // Encode data field
        encoded_message.insert(encoded_message.end(), data.begin(), data.end());

        return encoded_message;
    }

    static Message decode(const std::vector<uint8_t>& encoded_message) {
        // Decode source id (2 bytes)
        uint16_t src_id = (encoded_message[0] << 8) + encoded_message[1];

        // Decode destination id (2 bytes)
        uint16_t dest_id = (encoded_message[2] << 8) + encoded_message[3];

        // Decode counter (2 bytes)
        uint16_t cnt = (encoded_message[4] << 8) + encoded_message[5];

        // Decode index (1 byte)
        uint8_t idx = encoded_message[6];

        // Decode sub index (1 byte)
        uint8_t sub_idx = encoded_message[7];

        // Decode data field
        std::vector<uint8_t> dt(encoded_message.begin() + 8, encoded_message.end());

        return Message(src_id, dest_id, cnt, idx, sub_idx, dt);
    }
};
```

#### Node class

Next I realized that I needed to decouple some of the functionality I thought into a node class. The node class would be responsible for sending/receiving UDP messages and responding to the test service. There are 2 ports used, one for receiving and one for sending. Later I added to the Node class the node ID and the counter field functionality. Also I added a function in the message class to check if the message is empty.

``` cpp
// Node.h

// Node.h

#ifndef NODE_H
#define NODE_H

#include "Message.h"
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
    bool send(Message& msg, const std::string& dest_ip, int dest_port);
    Message receive();
};

#endif // NODE_H

```

``` cpp
// Message.cpp

// ...

bool Message::isEmpty() const {
    return (source_id == 0 && destination_id == 0 && counter == 0 &&
            index == 0 && sub_index == 0 && data.empty());
}

// ...
```

#### Main node

The design has a main node and a series of secondary nodes. The main node initiates the connection and the secondary nodes should respond. For the main Node I asked chatGPT to create a new main node class based on the node class. The difference would be that the node class has one id and ports as it's commnunication settings and the main node has (id, ports) multiple settings for the secondary nodes.


> create a new main-node class  based on the  node class. The difference is that it has an aditional  variable array of 2 bytes id, send and receive ports with max 10 entries.  The send function has the id parameter and that  sends the message to the equivelant port. Same with the receive function.

``` cpp
// MainNode.h

#ifndef MAINNODE_H
#define MAINNODE_H

#include "Node.h"
#include <vector>

class MainNode {
private:
    std::vector<uint16_t> ids; // Array of 2-byte IDs
    std::vector<int> send_ports;
    std::vector<int> receive_ports;
    std::vector<Node> nodes;

public:
    MainNode(const std::vector<uint16_t>& ids, const std::vector<int>& send_ports, const std::vector<int>& receive_ports);

    bool initializeAll();
    bool send(uint16_t id, Message& msg, const std::string& dest_ip);
    Message receive(uint16_t id);
};

#endif // MAINNODE_H
```

### Python

Generating the code for python was straightforward, I added the process function on the next prompt and then I asked to solve an issue that I had.

> generate the message and node class for python


``` Python

class Message:
    def __init__(self, source_id, destination_id, counter, index, sub_index, data):
        self.source_id = source_id
        self.destination_id = destination_id
        self.counter = counter
        self.index = index
        self.sub_index = sub_index
        self.data = data

    def encode(self):
        encoded_message = bytearray()
        encoded_message.extend(self.source_id.to_bytes(2, 'big'))
        encoded_message.extend(self.destination_id.to_bytes(2, 'big'))
        encoded_message.extend(self.counter.to_bytes(2, 'big'))
        encoded_message.append(self.index)
        encoded_message.append(self.sub_index)
        encoded_message.extend(self.data)
        return encoded_message

    @staticmethod
    def decode(encoded):
        source_id = int.from_bytes(encoded[:2], 'big')
        destination_id = int.from_bytes(encoded[2:4], 'big')
        counter = int.from_bytes(encoded[4:6], 'big')
        index = encoded[6]
        sub_index = encoded[7]
        data = encoded[8:]
        return Message(source_id, destination_id, counter, index, sub_index, data)

    def is_empty(self):
        return all(v == 0 for v in [self.source_id, self.destination_id, self.counter, self.index, self.sub_index]) and len(self.data) == 0


import socket

class Node:
    def __init__(self, ip, send_port, receive_port, id):
        self.ip = ip
        self.send_port = send_port
        self.receive_port = receive_port
        self.id = id
        self.send_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.receive_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.counter = 0

    def initialize(self):
        try:
            self.receive_socket.bind((self.ip, self.receive_port))
            return True
        except Exception as e:
            print(f"Failed to bind receiving socket: {e}")
            return False

    def send(self, msg, dest_ip):
        msg.set_source_id(self.id)
        msg.set_counter(self.counter)
        encoded_msg = msg.encode()
        self.send_socket.sendto(encoded_msg, (dest_ip, self.send_port))
        self.counter += 1

    def receive(self):
        data, addr = self.receive_socket.recvfrom(1024)
        return Message.decode(data)
```

> I apologize for the confusion. It appears that there's a typo in the receive function of the Node class. It's trying to call message.decode(data) instead of Message.decode(data).

> To fix this, you should replace message.decode(data) with Message.decode(data) in the receive function.

### Rust

The same was for rust, please check the prompt chat or the code in the github for more information. I had to solve some issues but it's more because of my lack of experience with Rust than anything else.

 I asked chatGPT on how to run the code since I'm not familiar with Rust and I got a comprehensive response.

> To run Rust code, you'll need to have the Rust compiler and Cargo (Rust's package manager) installed on your system. Here's how you can do it:

1. **Install Rust and Cargo**:
   - Visit the official Rust website: [https://www.rust-lang.org/tools/install](https://www.rust-lang.org/tools/install)
   - Follow the instructions for your operating system.

2. **Create a New Rust Project**:
   - Open your terminal or command prompt.
   - Navigate to the directory where you want to create your Rust project.
   - Run the command: `cargo new your_project_name --bin`
     - This will create a new Rust project with a binary (executable) target.

3. **Add Your Code**:
   - Open the generated folder (e.g., `your_project_name`) in a code editor.
   - Replace the contents of `src/main.rs` with the code provided in the previous message.

4. **Run the Code**:
   - Open a terminal or command prompt.
   - Navigate to your project directory (where the `Cargo.toml` file is located).
   - Run the command: `cargo run`
     - This will compile and run your Rust program.

> Remember to replace `your_project_name` with an appropriate name for your project. This will create a new Rust project with a binary target and you can run it using `cargo run`.

## Takeaways

I found out that after I while I didn't trust the AI to correct small changes while keeping the code it works the code. SO any small changes that was needed I did them manually.

I really like this way of development, it keeps the fun part while remove the boring part of writing repititive/boilerplate code. It also gives me a sense of better communicating with the machine, there is some sort of dialog which makes it metaphusically interesting. I'm not sure that I can go back to the regular way of working, pressing dots and semicolons. It is definetely exciting.

I also tried to use zig, a language that I'm totally unfamiliar but it didn't work and I didn't have the patience to try and make it work. From a quick look it doesn't offer UDP connectivity yet.

In the end, as I said it was fun. Keeping the complexity of the code down, will also make using AI much faster.

## Used versions

| Tool | Version   |
|----------|------|
| Ubuntu | 20.04 LTS |
| AI | ChatGPT 3.5 Aug 3 version |
| Rust | 1.72.0 |
| Python | 3.8.10 |
| C/C++ | gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.1) |
