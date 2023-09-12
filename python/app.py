from message import Message
from node import Node
import time

print("Hello.")

# Example usage
node = Node("127.0.0.1", 4025, 4026, 11)

if node.initialize():
    while True:

        # Receive a message
        received_msg = node.receive()

        if not received_msg.is_empty():
            print("Received Message:")
            print(f"Source ID: {received_msg.source_id}")
            print(f"Destination ID: {received_msg.destination_id}")
            print(f"Counter: {received_msg.counter}")
            print(f"Index: {received_msg.index}")
            print(f"Sub Index: {received_msg.sub_index}")
            print(f"Data: {received_msg.data}")

            node.process_message(received_msg)

else:
    print("Failed to initialize node.")
