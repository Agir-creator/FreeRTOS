#pragma once
#include "rpc_types.h"
#include "rpc_data_link.h"
#include <map>
#include <string>
#include <functional>
#include <memory>
#include <stdexcept>

namespace RPC {

class TransportLayer {
private:
    std::unique_ptr<DataLinkLayer> data_link_layer_;
    std::map<std::string, RpcFunction> registered_functions_;
    std::map<uint8_t, std::vector<uint8_t>> pending_responses_;
    uint8_t next_sequence_number_ = 0;
    Config config_;
    
    std::vector<uint8_t> serialize_message(const TransportMessage& msg) const;
    TransportMessage deserialize_message(const std::vector<uint8_t>& data) const;
    
public:
    TransportLayer(std::unique_ptr<DataLinkLayer> data_link_layer, const Config& config = Config{});
    
    void register_function(const std::string& name, RpcFunction function);
    std::vector<uint8_t> call_remote_function(const std::string& function_name, 
                                             const std::vector<uint8_t>& arguments,
                                             uint32_t timeout_ms = 1000);
    void process_incoming_messages();
};

} 