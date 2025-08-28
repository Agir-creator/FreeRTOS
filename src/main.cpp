#include "rpc_application.h"
#include "rpc_physical.h"
#include "rpc_data_link.h"
#include "rpc_transport.h"
#include <iostream>
#include <memory>


std::vector<uint8_t> add_numbers(const std::vector<uint8_t>& args) {
    std::cout << "add_numbers called with: ";
    for (auto arg : args) std::cout << (int)arg << " ";
    std::cout << std::endl;
    
    if (args.size() >= 2) {
        uint8_t result = args[0] + args[1];
        return {result};
    }
    return {};
}

std::vector<uint8_t> get_status(const std::vector<uint8_t>& args) {
    std::cout << "get_status called" << std::endl;
    return {0x01, 0x02, 0x03};
}

int main() {
    std::cout << "=== RPC Protocol Demo ===" << std::endl;
    
    try {
        
        auto physical = std::make_unique<RPC::UartPhysicalLayer>();
        auto data_link = std::make_unique<RPC::DataLinkLayer>(std::move(physical));
        auto transport = std::make_unique<RPC::TransportLayer>(std::move(data_link));
        auto application = std::make_unique<RPC::ApplicationLayer>(std::move(transport));
        
        
        physical->init();
        application->init();
        
        
        transport->register_function("add", add_numbers);
        transport->register_function("status", get_status);
        
        
        std::cout << "\n=== Testing RPC Calls ===" << std::endl;
        
        
        std::vector<uint8_t> args = {5, 3};
        auto result = transport->call_remote_function("add", args);
        
        std::cout << "Result: ";
        for (auto byte : result) std::cout << (int)byte << " ";
        std::cout << std::endl;
        
        
        auto status = transport->call_remote_function("status", {});
        std::cout << "Status: ";
        for (auto byte : status) std::cout << (int)byte << " ";
        std::cout << std::endl;
        
        std::cout << "\n=== Demo Completed ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
