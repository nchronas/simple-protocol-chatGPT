// Message.cpp

#include "message.h"

Message::Message(uint16_t source_id, uint16_t destination_id, uint16_t counter, uint8_t index, uint8_t sub_index, const std::vector<uint8_t>& data)
    : source_id(source_id), destination_id(destination_id), counter(counter), index(index), sub_index(sub_index), data(data) {}

uint16_t Message::getSourceID() const {
    return source_id;
}

uint16_t Message::getDestinationID() const {
    return destination_id;
}

uint16_t Message::getCounter() const {
    return counter;
}

uint8_t Message::getIndex() const {
    return index;
}

uint8_t Message::getSubIndex() const {
    return sub_index;
}

const std::vector<uint8_t>& Message::getData() const {
    return data;
}

void Message::setSourceID(uint16_t id) {
    source_id = id;
}

void Message::setDestinationID(uint16_t id) {
    destination_id = id;
}

void Message::setCounter(uint16_t count) {
    counter = count;
}

void Message::setIndex(uint8_t idx) {
    index = idx;
}

void Message::setSubIndex(uint8_t sub_idx) {
    sub_index = sub_idx;
}

void Message::setData(const std::vector<uint8_t>& d) {
    data = d;
}

std::vector<uint8_t> Message::encode() const {
    // Implement encoding logic here
    // This will depend on the specific encoding format you choose
    // For simplicity, let's assume a basic encoding for now
    // You may need to replace this with your actual encoding logic
    std::vector<uint8_t> encoded_message;

    // Add source id
    encoded_message.push_back((source_id >> 8) & 0xFF);
    encoded_message.push_back(source_id & 0xFF);

    // Add destination id
    encoded_message.push_back((destination_id >> 8) & 0xFF);
    encoded_message.push_back(destination_id & 0xFF);

    // Add counter
    encoded_message.push_back((counter >> 8) & 0xFF);
    encoded_message.push_back(counter & 0xFF);

    // Add index and sub index
    encoded_message.push_back(index);
    encoded_message.push_back(sub_index);

    // Add data
    encoded_message.insert(encoded_message.end(), data.begin(), data.end());

    return encoded_message;
}

Message Message::decode(const std::vector<uint8_t>& encoded) {
    // Implement decoding logic here
    // This will depend on the specific encoding format you choose
    // For simplicity, let's assume a basic decoding for now
    // You may need to replace this with your actual decoding logic

    uint16_t source_id = (encoded[0] << 8) | encoded[1];
    uint16_t destination_id = (encoded[2] << 8) | encoded[3];
    uint16_t counter = (encoded[4] << 8) | encoded[5];
    uint8_t index = encoded[6];
    uint8_t sub_index = encoded[7];

    std::vector<uint8_t> data(encoded.begin() + 8, encoded.end());

    return Message(source_id, destination_id, counter, index, sub_index, data);
}

bool Message::isEmpty() const {
    return source_id == 0 && destination_id == 0 && counter == 0 && index == 0 && sub_index == 0 && data.empty();
}
