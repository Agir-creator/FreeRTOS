#pragma once
#include "rpc_types.h"
#include <vector>
#include <memory>

namespace RPC {

class PhysicalLayer {
public:
    virtual ~PhysicalLayer() = default;
    
    virtual bool init() = 0;
    virtual bool send(const std::vector<uint8_t>& data) = 0;
    virtual std::vector<uint8_t> receive(uint32_t timeout_ms) = 0;
    virtual bool data_available() = 0;
};

class UartPhysicalLayer : public PhysicalLayer {
private:
    void* uart_handle_;
    uint32_t baud_rate_;
    
public:
    UartPhysicalLayer(void* uart_handle = nullptr, uint32_t baud_rate = 115200);
    bool init() override;
    bool send(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> receive(uint32_t timeout_ms) override;
    bool data_available() override;
};

} 