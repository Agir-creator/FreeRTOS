#pragma once
#include <cstdint>
#include <cstring>
#include <functional>
#include <vector>
#include <string>

namespace RPC {
    

enum class MessageType : uint8_t {
    REQUEST = 0x0B,    
    RESPONSE = 0x0C,   
    NOTIFY = 0x16,     
    ERROR = 0x21       
};


struct DataLinkPacket {
    uint8_t start_byte1 = 0xFA;
    uint16_t length;
    uint8_t header_crc;
    uint8_t start_byte2 = 0xFB;
    std::vector<uint8_t> payload;
    uint8_t payload_crc;
    uint8_t end_byte = 0xFB;
};


struct TransportMessage {
    MessageType type;
    uint8_t sequence_number;
    std::string function_name;
    std::vector<uint8_t> arguments;
};


using RpcFunction = std::function<std::vector<uint8_t>(const std::vector<uint8_t>&)>;


struct Config {
    uint32_t uart_timeout_ms = 100;
    uint32_t response_timeout_ms = 1000;
    size_t max_packet_size = 256;
};

} 
