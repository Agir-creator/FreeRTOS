#include "rpc_data_link.h"
#include <iostream>
#include <thread>

namespace RPC {

DataLinkLayer::DataLinkLayer(std::unique_ptr<PhysicalLayer> physical_layer, const Config& config)
    : physical_layer_(std::move(physical_layer)), config_(config) {}

uint8_t DataLinkLayer::calculate_crc8(const std::vector<uint8_t>& data) const {
    uint8_t crc = 0;
    for (uint8_t byte : data) {
        crc ^= byte;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

bool DataLinkLayer::validate_packet(const DataLinkPacket& packet) const {
    if (packet.start_byte1 != 0xFA || packet.start_byte2 != 0xFB || packet.end_byte != 0xFB) {
        return false;
    }
    
    std::vector<uint8_t> header = {packet.start_byte1, 
                                  static_cast<uint8_t>(packet.length & 0xFF),
                                  static_cast<uint8_t>((packet.length >> 8) & 0xFF)};
    return calculate_crc8(header) == packet.header_crc;
}

bool DataLinkLayer::send_packet(const std::vector<uint8_t>& payload) {
    DataLinkPacket packet;
    packet.length = payload.size();
    
    std::vector<uint8_t> header = {packet.start_byte1, 
                                  static_cast<uint8_t>(packet.length & 0xFF),
                                  static_cast<uint8_t>((packet.length >> 8) & 0xFF)};
    packet.header_crc = calculate_crc8(header);
    packet.payload = payload;
    
    std::vector<uint8_t> payload_for_crc = packet.payload;
    packet.payload_crc = calculate_crc8(payload_for_crc);
    
    std::vector<uint8_t> serialized;
    serialized.push_back(packet.start_byte1);
    serialized.push_back(static_cast<uint8_t>(packet.length & 0xFF));
    serialized.push_back(static_cast<uint8_t>((packet.length >> 8) & 0xFF));
    serialized.push_back(packet.header_crc);
    serialized.push_back(packet.start_byte2);
    serialized.insert(serialized.end(), packet.payload.begin(), packet.payload.end());
    serialized.push_back(packet.payload_crc);
    serialized.push_back(packet.end_byte);
    
    return physical_layer_->send(serialized);
}

std::vector<uint8_t> DataLinkLayer::receive_packet(uint32_t timeout_ms) {
    std::cout << "Receiving packet (timeout: " << timeout_ms << "ms)" << std::endl;
    return {}; 
}

void DataLinkLayer::reception_task() {
    while (true) {
        auto payload = receive_packet(1000);
        if (!payload.empty()) {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            receive_queue_.push(payload);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

} 