#pragma once
#include "rpc_types.h"
#include "rpc_physical.h"
#include <memory>
#include <queue>
#include <mutex>

namespace RPC {

class DataLinkLayer {
private:
    std::unique_ptr<PhysicalLayer> physical_layer_;
    std::queue<std::vector<uint8_t>> receive_queue_;
    std::mutex queue_mutex_;
    Config config_;
    
    uint8_t calculate_crc8(const std::vector<uint8_t>& data) const;
    bool validate_packet(const DataLinkPacket& packet) const;
    
public:
    DataLinkLayer(std::unique_ptr<PhysicalLayer> physical_layer, const Config& config = Config{});
    
    bool send_packet(const std::vector<uint8_t>& payload);
    std::vector<uint8_t> receive_packet(uint32_t timeout_ms);
    void reception_task();
};

} 