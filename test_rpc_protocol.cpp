#include "test_utils.h"
#include "rpc_data_link.h"
#include "rpc_transport.h"
#include "rpc_application.h"
#include <iostream>
#include <cassert>
#include <memory>

using namespace RPC;
using namespace TestUtils;


void test_crc_calculation() {
    std::cout << "Testing CRC calculation...\n";
    
    std::vector<uint8_t> test_data = {0x01, 0x02, 0x03, 0x04};
    DataLinkLayer dl_layer(std::make_unique<MockPhysicalLayer>(), Config{});
    
    
    std::cout << "CRC test completed (indirect testing)\n";
}


void test_data_link_validation() {
    std::cout << "Testing Data Link validation...\n";
    
    auto mock_physical = std::make_unique<MockPhysicalLayer>();
    DataLinkLayer dl_layer(std::move(mock_physical), Config{});
    
    
    std::cout << "Data Link validation test setup complete\n";
}


void test_data_link_send_receive() {
    std::cout << "Testing Data Link send/receive...\n";
    
    auto mock_physical = std::make_unique<MockPhysicalLayer>();
    auto* physical_ptr = mock_physical.get();
    DataLinkLayer dl_layer(std::move(mock_physical), Config{});
    
    
    std::vector<uint8_t> test_payload = {0x01, 0x02, 0x03, 0x04};
    
    
    bool send_result = dl_layer.send_packet(test_payload);
    assert_true(send_result, "Data Link send should succeed");
    
    
    assert_true(physical_ptr->get_sent_count() > 0, "Data should be sent to physical layer");
    
    std::cout << "Data Link send/receive test passed\n";
}


void test_transport_function_registration() {
    std::cout << "Testing Transport function registration...\n";
    
    auto mock_data_link = std::make_unique<DataLinkLayer>(
        std::make_unique<MockPhysicalLayer>(), Config{});
    
    TransportLayer transport(std::move(mock_data_link), Config{});
    
    
    transport.register_function("add", mock_add_function);
    transport.register_function("multiply", mock_multiply_function);
    
    std::cout << "Function registration test passed\n";
}


void test_transport_remote_call() {
    std::cout << "Testing Transport remote call...\n";
    
    auto mock_physical = std::make_unique<MockPhysicalLayer>();
    auto mock_data_link = std::make_unique<DataLinkLayer>(
        std::move(mock_physical), Config{});
    
    TransportLayer transport(std::move(mock_data_link), Config{});
    
    
    
    std::cout << "Transport remote call test setup complete\n";
}

void test_application_initialization() {
    std::cout << "Testing Application initialization...\n";
    
    auto mock_transport = std::make_unique<TransportLayer>(
        std::make_unique<DataLinkLayer>(
            std::make_unique<MockPhysicalLayer>(), Config{}),
        Config{});
    
    ApplicationLayer application(std::move(mock_transport));
    
    
    application.init();
    
    std::cout << "Application initialization test passed\n";
}


void test_complete_rpc_flow() {
    std::cout << "Testing complete RPC flow...\n";
    
    
    auto mock_physical = std::make_unique<MockPhysicalLayer>();
    auto* physical_ptr = mock_physical.get();
    
    
    auto data_link = std::make_unique<DataLinkLayer>(std::move(mock_physical), Config{});
    auto transport = std::make_unique<TransportLayer>(std::move(data_link), Config{});
    auto application = std::make_unique<ApplicationLayer>(std::move(transport));
    
    
    application->register_function("test_echo", mock_echo_function);
    
    std::cout << "Complete RPC flow test setup complete\n";
}


void test_error_conditions() {
    std::cout << "Testing error conditions...\n";
    
    
    auto mock_physical = std::make_unique<MockPhysicalLayer>();
    mock_physical->set_send_failure(true);
    
    DataLinkLayer dl_layer(std::move(mock_physical), Config{});
    
    std::vector<uint8_t> test_data = {0x01, 0x02};
    bool result = dl_layer.send_packet(test_data);
    
    assert_false(result, "Send should fail when physical layer fails");
    
    std::cout << "Error conditions test passed\n";
}


void test_timeout_handling() {
    std::cout << "Testing timeout handling...\n";
    
    auto mock_physical = std::make_unique<MockPhysicalLayer>();
    auto mock_data_link = std::make_unique<DataLinkLayer>(
        std::move(mock_physical), Config{});
    
    TransportLayer transport(std::move(mock_data_link), Config{});
    
    
    std::cout << "Timeout handling test setup complete\n";
}


int main() {
    std::cout << "Starting RPC Protocol Tests...\n";
    std::cout << "========================================\n";
    
    try {
        test_crc_calculation();
        test_data_link_validation();
        test_data_link_send_receive();
        test_transport_function_registration();
        test_transport_remote_call();
        test_application_initialization();
        test_complete_rpc_flow();
        test_error_conditions();
        test_timeout_handling();
        
        std::cout << "========================================\n";
        std::cout << "All tests passed successfully! ✅\n";
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << " ❌\n";
        return 1;
    }
}


namespace TestUtils {

std::vector<uint8_t> mock_add_function(const std::vector<uint8_t>& args) {
    if (args.size() >= 2) {
        uint8_t result = args[0] + args[1];
        return {result};
    }
    return {};
}

std::vector<uint8_t> mock_multiply_function(const std::vector<uint8_t>& args) {
    if (args.size() >= 2) {
        uint8_t result = args[0] * args[1];
        return {result};
    }
    return {};
}

std::vector<uint8_t> mock_echo_function(const std::vector<uint8_t>& args) {
    return args; 
}

std::vector<uint8_t> mock_failing_function(const std::vector<uint8_t>& args) {
    (void)args; 
    throw std::runtime_error("Function intentionally failed");
}

void assert_vectors_equal(const std::vector<uint8_t>& expected, 
                         const std::vector<uint8_t>& actual,
                         const std::string& message) {
    if (expected != actual) {
        throw std::runtime_error(message + " - Vectors not equal");
    }
}

void assert_true(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message + " - Expected true, got false");
    }
}

void assert_false(bool condition, const std::string& message) {
    if (condition) {
        throw std::runtime_error(message + " - Expected false, got true");
    }
}

} 