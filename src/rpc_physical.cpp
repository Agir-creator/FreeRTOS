#include "rpc_physical.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace RPC {

UartPhysicalLayer::UartPhysicalLayer(void* uart_handle, uint32_t baud_rate)
    : uart_handle_(uart_handle), baud_rate_(baud_rate) {}

bool UartPhysicalLayer::init() {
    std::cout << "UART initialized with baud rate: " << baud_rate_ << std::endl;
    return true;
}

bool UartPhysicalLayer::send(const std::vector<uint8_t>& data) {
    std::cout << "Sending data: ";
    for (auto byte : data) {
        printf("%02X ", byte);
    }
    std::cout << std::endl;
    return true;
}

std::vector<uint8_t> UartPhysicalLayer::receive(uint32_t timeout_ms) {
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    return {}; 
}

bool UartPhysicalLayer::data_available() {
    return false; 
}

} 