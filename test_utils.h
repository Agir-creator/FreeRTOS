#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <cstdint>
#include <vector>
#include <string>
#include "rpc_types.h"

namespace TestUtils {


class MockPhysicalLayer : public RPC::PhysicalLayer {
private:
    std::vector<std::vector<uint8_t>> sent_data_;
    std::vector<std::vector<uint8_t>> receive_queue_;
    bool should_fail_send_ = false;
    bool should_fail_receive_ = false;

public:
    MockPhysicalLayer() = default;

    bool init() override { return true; }

    bool send(const std::vector<uint8_t>& data) override {
        if (should_fail_send_) return false;
        sent_data_.push_back(data);
        return true;
    }

    std::vector<uint8_t> receive(uint32_t timeout_ms) override {
        (void)timeout_ms; 
        if (should_fail_receive_ || receive_queue_.empty()) {
            return {};
        }
        
        auto data = receive_queue_.front();
        receive_queue_.erase(receive_queue_.begin());
        return data;
    }

    bool data_available() override {
        return !receive_queue_.empty();
    }

    
    void queue_receive_data(const std::vector<uint8_t>& data) {
        receive_queue_.push_back(data);
    }

    const std::vector<std::vector<uint8_t>>& get_sent_data() const {
        return sent_data_;
    }

    void clear_sent_data() {
        sent_data_.clear();
    }

    void set_send_failure(bool fail) {
        should_fail_send_ = fail;
    }

    void set_receive_failure(bool fail) {
        should_fail_receive_ = fail;
    }

    size_t get_sent_count() const {
        return sent_data_.size();
    }
};


std::vector<uint8_t> create_test_payload(const std::vector<uint8_t>& transport_data);
std::vector<uint8_t> create_valid_data_link_packet(const std::vector<uint8_t>& payload);
std::vector<uint8_t> create_corrupted_data_link_packet(const std::vector<uint8_t>& payload);

s
std::vector<uint8_t> mock_add_function(const std::vector<uint8_t>& args);
std::vector<uint8_t> mock_multiply_function(const std::vector<uint8_t>& args);
std::vector<uint8_t> mock_echo_function(const std::vector<uint8_t>& args);
std::vector<uint8_t> mock_failing_function(const std::vector<uint8_t>& args);


void assert_vectors_equal(const std::vector<uint8_t>& expected, 
                         const std::vector<uint8_t>& actual,
                         const std::string& message);

void assert_true(bool condition, const std::string& message);
void assert_false(bool condition, const std::string& message);

} 

#endif 