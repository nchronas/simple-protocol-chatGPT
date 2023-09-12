const std = @import("std");
const Message = @import("Message.zig").Message;
const Node = @import("Node.zig").Node;

pub fn main() void {

    std.debug.print("{}\n", .{"Hello."});
    const node = try Node.init();

    while (true) : (true) {

        const receivedMsg = try node.receive();
        node.processMessage(&receivedMsg);
        std.debug.print("{}\n", .{receivedMsg});
    }
}
