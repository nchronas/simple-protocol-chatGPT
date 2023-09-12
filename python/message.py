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

    def set_counter(self, cnt):
        self.counter = cnt

    def is_empty(self):
        return all(v == 0 for v in [self.source_id, self.destination_id, self.counter, self.index, self.sub_index]) and len(self.data) == 0
