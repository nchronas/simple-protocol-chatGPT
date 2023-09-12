// Message.h

#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <cstdint>

class Message {
private:
    uint16_t source_id;
    uint16_t destination_id;
    uint16_t counter;
    uint8_t index;
    uint8_t sub_index;
    std::vector<uint8_t> data;

public:
    Message(uint16_t source_id, uint16_t destination_id, uint16_t counter, uint8_t index, uint8_t sub_index, const std::vector<uint8_t>& data);

    uint16_t getSourceID() const;
    uint16_t getDestinationID() const;
    uint16_t getCounter() const;
    uint8_t getIndex() const;
    uint8_t getSubIndex() const;
    const std::vector<uint8_t>& getData() const;

    void setSourceID(uint16_t id);
    void setDestinationID(uint16_t id);
    void setCounter(uint16_t count);
    void setIndex(uint8_t idx);
    void setSubIndex(uint8_t sub_idx);
    void setData(const std::vector<uint8_t>& d);

    std::vector<uint8_t> encode() const;
    static Message decode(const std::vector<uint8_t>& encoded);
    
    bool isEmpty() const; // Check if the message is empty
};

#endif // MESSAGE_H
