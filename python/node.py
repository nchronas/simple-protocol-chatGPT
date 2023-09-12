import socket
from message import Message

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

    def send(self, msg):
        msg.set_counter(self.counter)
        encoded_msg = msg.encode()
        self.counter += 1
        self.send_socket.sendto(encoded_msg, (self.ip, self.send_port))

    def receive(self):
        data, addr = self.receive_socket.recvfrom(1024)
        return Message.decode(data)

    def process_message(self, msg):
        if msg.index == 17 and msg.sub_index == 1:
            # Create a response message
            response_msg = Message(
                source_id=msg.destination_id,
                destination_id=msg.source_id,
                counter=0,
                index=17,
                sub_index=2,
                data=[]
            )
            self.send(response_msg)