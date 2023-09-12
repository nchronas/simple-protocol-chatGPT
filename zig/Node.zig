const std = @import("std");
const Message = @import("Message.zig").Message;

pub const Node = struct {

    pub const NodeId: u16 = 0x1234;
    pub const SendPort: u16 = 1025;
    pub const ReceivePort: u16 = 1026;
    pub const MaxDataSize: usize = 10;

    pub var ip: []const u8 = "127.0.0.1";
    pub var sendSocket: std.net.UdpSocket = undefined;
    pub var receiveSocket: std.net.UdpSocket = undefined;
    pub var counter: u16 = 0;

    pub fn init() !void {
        try sendSocket.init(std.net.AddressFamily.ipv4, std.net.SocketType.datagram, std.net.Protocol.udp);
        try receiveSocket.init(std.net.AddressFamily.ipv4, std.net.SocketType.datagram, std.net.Protocol.udp);

        const bindAddr = try std.net.ipv4Address("0.0.0.0");
        try sendSocket.bind(std.net.SocketAddr{ .address = bindAddr, .port = SendPort });
        try receiveSocket.bind(std.net.SocketAddr{ .address = bindAddr, .port = ReceivePort });

        const destAddr = try std.net.ipv4Address(ip);
        try receiveSocket.connect(std.net.SocketAddr{ .address = destAddr, .port = SendPort });
        return;
    }

    pub fn send(msg: *Message) !void {
        msg.sourceId = NodeId;
        msg.counter = counter;
        const encoded = msg.encode();
        try sendSocket.send(encoded);
        counter += 1;
    }

    pub fn receive() !Message {
        var buffer: [20]u8 = undefined;
        const n = try receiveSocket.receive(buffer);
        return Message.decode(buffer[0..n]);
    }

    pub fn processMessage(msg: *Message) void {
        if (msg.index == 17 and msg.subIndex == 1) {
            msg.index = 17;
            msg.subIndex = 2;
            msg.destinationId = NodeId;
            msg.sourceId = NodeId;
            msg.counter = 0;
            msg.data = undefined;
            try send(msg);
        }
    }
};
