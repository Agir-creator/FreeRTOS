#include "rpc_application.h"
#include <iostream>
#include <thread>

namespace RPC {

ApplicationLayer::ApplicationLayer(std::unique_ptr<TransportLayer> transport_layer)
    : transport_layer_(std::move(transport_layer)) {}

void ApplicationLayer::init() {
    std::cout << "Application layer initialized" << std::endl;
}

void ApplicationLayer::example_usage() {
    std::cout << "RPC Application Example" << std::endl;
}

void ApplicationLayer::reception_task(void* parameter) {
    auto app = static_cast<ApplicationLayer*>(parameter);
    while (true) {
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

} 