#pragma once
#include "rpc_transport.h"
#include <memory>

namespace RPC {

class ApplicationLayer {
private:
    std::unique_ptr<TransportLayer> transport_layer_;
    
public:
    ApplicationLayer(std::unique_ptr<TransportLayer> transport_layer);
    
    void init();
    void example_usage();
    static void reception_task(void* parameter);
};

} 