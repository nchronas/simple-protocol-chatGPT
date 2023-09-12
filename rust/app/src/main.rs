mod message;
mod node;

use node::Node;

fn main() {
    let mut node = Node::new("127.0.0.1", 5025, 5026, 12);

    if node.initialize().is_ok() {
        loop {
            // Receive a message
            let received_msg = node.receive().expect("Failed to receive message");

            if !received_msg.is_empty() {
                println!("Received Message:");
                println!("Source ID: {}", received_msg.source_id);
                println!("Destination ID: {}", received_msg.destination_id);
                println!("Counter: {}", received_msg.counter);
                println!("Index: {}", received_msg.index);
                println!("Sub Index: {}", received_msg.sub_index);
                println!("Data: {:?}", received_msg.data);
            }

            // Process the received message
            node.process_message(&received_msg);
        }
    } else {
        println!("Failed to initialize node.");
    }
}
