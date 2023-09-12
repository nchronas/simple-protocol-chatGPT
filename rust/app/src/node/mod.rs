use crate::message::Message;
use std::net::{UdpSocket, SocketAddr};

#[derive(Debug)]
pub struct Node {
    pub ip: String,
    pub send_port: u16,
    pub receive_port: u16,
    pub id: u16,
    pub send_socket: UdpSocket,
    pub receive_socket: UdpSocket,
    pub counter: u16,
}

impl Node {
    pub fn new(ip: &str, send_port: u16, receive_port: u16, id: u16) -> Node {
        let send_socket = UdpSocket::bind("0.0.0.0:0").expect("Failed to bind send socket");
        let receive_socket = UdpSocket::bind(format!("{}:{}", ip, receive_port)).expect("Failed to bind receive socket");

        Node { ip: ip.to_string(), send_port, receive_port, id, send_socket, receive_socket, counter: 0 }
    }

    pub fn initialize(&self) -> std::io::Result<()> {
        Ok(())
    }

    pub fn send(&mut self, msg: &Message) -> std::io::Result<()> {
        let encoded_msg = msg.encode();
        let dest_addr = format!("{}:{}", self.ip, self.send_port);
        let dest_sock_addr: SocketAddr = dest_addr.parse().expect("Invalid destination address");

        self.send_socket.send_to(&encoded_msg, dest_sock_addr)?;
        self.counter += 1;

        Ok(())
    }

    pub fn receive(&mut self) -> std::io::Result<Message> {
        let mut buffer = [0; 1024];
        let (received_bytes, _) = self.receive_socket.recv_from(&mut buffer)?;

        let received_data = buffer[..received_bytes].to_vec();
        match Message::decode(&received_data) {
            Some(msg) => Ok(msg),
            None => Err(std::io::Error::new(std::io::ErrorKind::InvalidData, "Failed to decode message")),
        }
    }

    pub fn process_message(&mut self, msg: &Message) {
        if msg.index == 17 && msg.sub_index == 1 {
            // Create a response message
            let response_msg = Message::new(
                msg.destination_id,
                msg.source_id,
                0,
                17,
                2,
                vec![],
            );

            // Send the response
            let _ = self.send(&response_msg);
        }
    }
}
