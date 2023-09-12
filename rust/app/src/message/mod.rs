#[derive(Debug)]
pub struct Message {
    pub source_id: u16,
    pub destination_id: u16,
    pub counter: u16,
    pub index: u8,
    pub sub_index: u8,
    pub data: Vec<u8>,
}

impl Message {
    pub fn new(source_id: u16, destination_id: u16, counter: u16, index: u8, sub_index: u8, data: Vec<u8>) -> Message {
        Message { source_id, destination_id, counter, index, sub_index, data }
    }

    pub fn encode(&self) -> Vec<u8> {
        let mut encoded_message = vec![];
        encoded_message.extend_from_slice(&self.source_id.to_be_bytes());
        encoded_message.extend_from_slice(&self.destination_id.to_be_bytes());
        encoded_message.extend_from_slice(&self.counter.to_be_bytes());
        encoded_message.push(self.index);
        encoded_message.push(self.sub_index);
        encoded_message.extend_from_slice(&self.data);
        encoded_message
    }

    pub fn decode(encoded: &[u8]) -> Option<Message> {
        if encoded.len() < 8 {
            return None;
        }

        let source_id = u16::from_be_bytes([encoded[0], encoded[1]]);
        let destination_id = u16::from_be_bytes([encoded[2], encoded[3]]);
        let counter = u16::from_be_bytes([encoded[4], encoded[5]]);
        let index = encoded[6];
        let sub_index = encoded[7];
        let data = encoded[8..].to_vec();

        Some(Message { source_id, destination_id, counter, index, sub_index, data })
    }

    pub fn is_empty(&self) -> bool {
        self.source_id == 0 && self.destination_id == 0 && self.counter == 0 && self.index == 0 && self.sub_index == 0 && self.data.is_empty()
    }
}
