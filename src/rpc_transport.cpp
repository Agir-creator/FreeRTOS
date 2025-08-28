#include "rpc_transport.h"
#include <iostream>
#include <thread>

namespace RPC {

TransportLayer::TransportLayer(std::unique_ptr<DataLinkLayer> data_link_layer, const Config& config)
    : data_link_layer_(std::move(data_link_layer)), config_(config) {}

void TransportLayer::register_function(const std::string& name, RpcFunction function) {
    registered_functions_[name] = function;
    std::cout << "Registered function: " << name << std::endl;
}

std::vector<uint8_t> TransportLayer::serialize_message(const TransportMessage& msg) const {
    std::vector<uint8_t> result;
    result.push_back(static_cast<uint8_t>(msg.type));
    result.push_back(msg.sequence_number);
    result.insert(result.end(), msg.function_name.begin(), msg.function_name.end());
    result.push_back(0);
    result.insert(result.end(), msg.arguments.begin(), msg.arguments.end());
    return result;
}

TransportMessage TransportLayer::deserialize_message(const std::vector<uint8_t>& data) const {
    if (data.size() < 3) throw std::runtime_error("Invalid message size");
    
    TransportMessage msg;
    msg.type = static_cast<MessageType>(data[0]);
    msg.sequence_number = data[1];
    
    size_t terminator_pos = 2;
    while (terminator_pos < data.size() && data[terminator_pos] != 0) {
        terminator_pos++;
    }
    
    if (terminator_pos >= data.size()) throw std::runtime_error("Terminator not found");
    
    msg.function_name.assign(data.begin() + 2, data.begin() + terminator_pos);
    
    if (terminator_pos + 1 < data.size()) {
        msg.arguments.assign(data.begin() + terminator_pos + 1, data.end());
    }
    
    return msg;
}

std::vector<uint8_t> TransportLayer::call_remote_function(const std::string& function_name,
                                                         const std::vector<uint8_t>& arguments,
                                                         uint32_t timeout_ms) {
    TransportMessage request;
    request.type = MessageType::REQUEST;
    request.sequence_number = next_sequence_number_++;
    request.function_name = function_name;
    request.arguments = arguments;
    
    auto serialized = serialize_message(request);
    if (!data_link_layer_->send_packet(serialized)) {
        throw std::runtime_error("Send failed");
    }
    
    std::cout << "Called remote function: " << function_name << std::endl;
    return {0x01, 0x02, 0x03}; 
}

void TransportLayer::process_incoming_messages() {
    while (true) {
        try {
            auto payload = data_link_layer_->receive_packet(config_.uart_timeout_ms);
            if (!payload.empty()) {
                auto message = deserialize_message(payload);
                std::cout << "Received message for function: " << message.function_name << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error processing message: " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

} 