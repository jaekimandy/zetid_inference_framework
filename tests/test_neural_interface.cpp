/**
 * ZeticML Assignment - Neural Network Interface Unit Tests
 * Tests neural network interface implementations using doctest framework
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/neural_network_interface.h"
#include "../src/model_registry.h"
#include "../src/linear_regression.h"
#include "../src/logistic_regression.h"
#include "../src/multi_class_classifier.h"
#include "../src/two_layer_mlp.h"
#include "../src/test_data_loader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

// ==================== Test Data Structure ====================

struct TestCase {
    std::vector<float> input;
    std::vector<float> parameters;
    std::vector<float> expected_output;
    std::string description;
};

// ==================== File Data Loader ====================

std::vector<TestCase> load_test_cases(const std::string& filename) {
    std::vector<TestCase> cases;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Warning: Could not open test file: " << filename << std::endl;
        return cases;
    }

    std::string line;
    int line_number = 0;

    while (std::getline(file, line)) {
        line_number++;
        if (line.empty() || line[0] == '#') continue;

        try {
            // Format: input1,input2,... | param1,param2,... | expected_output
            size_t pipe1 = line.find(" | ");
            size_t pipe2 = line.find(" | ", pipe1 + 3);
            if (pipe1 == std::string::npos || pipe2 == std::string::npos) continue;

            std::string input_str = line.substr(0, pipe1);
            std::string param_str = line.substr(pipe1 + 3, pipe2 - pipe1 - 3);
            std::string output_str = line.substr(pipe2 + 3);

            TestCase test_case;
            test_case.description = "Line " + std::to_string(line_number);

            // Parse input values
            std::istringstream input_stream(input_str);
            std::string value;
            while (std::getline(input_stream, value, ',')) {
                test_case.input.push_back(std::stof(value));
            }

            // Parse parameter values
            std::istringstream param_stream(param_str);
            while (std::getline(param_stream, value, ',')) {
                test_case.parameters.push_back(std::stof(value));
            }

            // Parse expected output values
            std::istringstream output_stream(output_str);
            while (std::getline(output_stream, value, ',')) {
                test_case.expected_output.push_back(std::stof(value));
            }

            cases.push_back(test_case);
        }
        catch (const std::exception& e) {
            std::cout << "Error parsing line " << line_number << ": " << e.what() << std::endl;
        }
    }

    std::cout << "Loaded " << cases.size() << " test cases from " << filename << std::endl;
    return cases;
}

// ==================== Unit Tests ====================

TEST_CASE("Linear Regression") {
    using namespace ZeticML;

    // Load test data from file
    auto test_cases = load_test_cases("../tests/data/linear_regression_demo.txt");
    REQUIRE(!test_cases.empty());

    // Create model using registry pattern
    auto& registry = get_model_registry();
    auto model = registry.create_model("linear", 3);

    // Test model metadata
    CHECK(model->input_size() == 3);
    CHECK(model->output_size() == 1);
    CHECK(model->get_model_type() == "Linear Regression");

    // Test each case from file
    for (const auto& test_case : test_cases) {
        model->set_parameters(test_case.parameters);
        auto output = model->forward(test_case.input);

        REQUIRE(output.size() == 1);
        REQUIRE(output.size() == test_case.expected_output.size());

        float diff = std::abs(output[0] - test_case.expected_output[0]);
        INFO("Test case: " << test_case.description);
        INFO("Expected: " << test_case.expected_output[0] << ", Got: " << output[0]);
        CHECK(diff < 0.01f);
    }
}

TEST_CASE("Logistic Regression") {
    using namespace ZeticML;

    // Load test data from file
    auto test_cases = load_test_cases("../tests/data/logistic_regression_demo.txt");
    REQUIRE(!test_cases.empty());

    // Create model using registry pattern
    auto& registry = get_model_registry();
    auto model = registry.create_model("logistic", 2);

    // Test model metadata
    CHECK(model->input_size() == 2);
    CHECK(model->output_size() == 1);
    CHECK(model->get_model_type() == "Logistic Regression");

    // Test each case from file
    for (const auto& test_case : test_cases) {
        model->set_parameters(test_case.parameters);
        auto output = model->forward(test_case.input);

        REQUIRE(output.size() == 1);
        REQUIRE(output.size() == test_case.expected_output.size());

        float diff = std::abs(output[0] - test_case.expected_output[0]);
        INFO("Test case: " << test_case.description);
        INFO("Expected: " << test_case.expected_output[0] << ", Got: " << output[0]);
        CHECK(diff < 0.01f);
    }
}

TEST_CASE("Multi-Class Classifier") {
    using namespace ZeticML;

    // Load test data from file
    auto test_cases = load_test_cases("../tests/data/multi_class_demo.txt");
    REQUIRE(!test_cases.empty());

    // Create model using registry pattern
    auto& registry = get_model_registry();
    auto model = registry.create_model("multiclass", 4, 3);  // 4 inputs, 3 classes

    // Test model metadata
    CHECK(model->input_size() == 4);
    CHECK(model->output_size() == 3);
    CHECK(model->get_model_type().find("Multi-Class Classifier") != std::string::npos);

    // Test each case from file
    for (const auto& test_case : test_cases) {
        model->set_parameters(test_case.parameters);
        auto output = model->forward(test_case.input);

        REQUIRE(output.size() == 3);
        REQUIRE(output.size() == test_case.expected_output.size());

        // Test that probabilities sum to approximately 1.0
        float sum = 0.0f;
        for (float prob : output) {
            sum += prob;
            CHECK(prob >= 0.0f);
            CHECK(prob <= 1.0f);
        }
        CHECK(std::abs(sum - 1.0f) < 0.01f);

        // Test expected output values (with tolerance for softmax calculations)
        for (size_t i = 0; i < output.size(); ++i) {
            float diff = std::abs(output[i] - test_case.expected_output[i]);
            INFO("Test case: " << test_case.description);
            INFO("Class " << i << " - Expected: " << test_case.expected_output[i] << ", Got: " << output[i] << ", Diff: " << diff);
            CHECK(diff < 0.05f);
        }
    }
}

TEST_CASE("Two-Layer MLP") {
    using namespace ZeticML;

    // Load test data from file
    auto test_cases = load_test_cases("../tests/data/two_layer_mlp_demo.txt");
    REQUIRE(!test_cases.empty());

    // Create model using registry pattern
    auto& registry = get_model_registry();
    auto model = registry.create_model("mlp", 2, 3, 2);  // 2 inputs, 3 hidden, 2 outputs

    // Test model metadata
    CHECK(model->input_size() == 2);
    CHECK(model->output_size() == 2);
    CHECK(model->get_model_type() == "Two-Layer MLP");

    // Test each case from file
    for (const auto& test_case : test_cases) {
        model->set_parameters(test_case.parameters);
        auto output = model->forward(test_case.input);

        REQUIRE(output.size() == 2);
        REQUIRE(output.size() == test_case.expected_output.size());

        // Test expected output values with tolerance for floating point calculations
        for (size_t i = 0; i < output.size(); ++i) {
            float diff = std::abs(output[i] - test_case.expected_output[i]);
            INFO("Test case: " << test_case.description);
            INFO("Output " << i << " - Expected: " << test_case.expected_output[i] << ", Got: " << output[i] << ", Diff: " << diff);
            CHECK(diff < 0.05f);
        }
    }
}

TEST_CASE("Registry Pattern Polymorphism") {
    using namespace ZeticML;

    auto& registry = get_model_registry();

    // Test registry functionality
    SUBCASE("Registry Registration") {
        CHECK(registry.is_registered("linear"));
        CHECK(registry.is_registered("logistic"));
        CHECK(registry.is_registered("multiclass"));
        CHECK(registry.is_registered("mlp"));
        CHECK(!registry.is_registered("nonexistent"));

        auto types = registry.get_registered_types();
        CHECK(types.size() == 4);
    }

    // Test polymorphic creation and usage
    SUBCASE("Polymorphic Model Creation") {
        struct TestConfig {
            std::string type;
            std::vector<size_t> dimensions;
            std::vector<float> parameters;
            size_t expected_input_size;
            size_t expected_output_size;
        };

        std::vector<TestConfig> test_configs = {
            {"linear", {2}, {0.5f, 0.3f, 0.1f}, 2, 1},
            {"logistic", {2}, {0.8f, -0.4f, 0.1f}, 2, 1},
            {"multiclass", {2, 3}, {0.5f, 0.3f, 0.1f, -0.2f, 0.6f, -0.1f, 0.1f, -0.4f, 0.2f}, 2, 3},
            {"mlp", {2, 3, 2}, std::vector<float>(17, 0.2f), 2, 2}
        };

        std::vector<float> test_input = {1.0f, 0.5f};

        // Pure polymorphic loop - same code tests all model types
        for (const auto& config : test_configs) {
            INFO("Testing model type: " << config.type);

            // Create model polymorphically using method overloads
            std::unique_ptr<NeuralNetwork> model;
            if (config.dimensions.size() == 1) {
                model = registry.create_model(config.type, config.dimensions[0]);
            } else if (config.dimensions.size() == 2) {
                model = registry.create_model(config.type, config.dimensions[0], config.dimensions[1]);
            } else if (config.dimensions.size() == 3) {
                model = registry.create_model(config.type, config.dimensions[0], config.dimensions[1], config.dimensions[2]);
            }

            // Test through base class interface only
            CHECK(model->input_size() == config.expected_input_size);
            CHECK(model->output_size() == config.expected_output_size);
            CHECK(!model->get_model_type().empty());

            // Test polymorphic method calls
            model->set_parameters(config.parameters);
            auto result = model->forward(test_input);
            CHECK(result.size() == config.expected_output_size);
        }
    }

    // Test error handling
    SUBCASE("Registry Error Handling") {
        CHECK_THROWS(registry.create_model("unknown_type", 2));
        CHECK_THROWS(registry.create_model("linear", 2, 3));  // Wrong parameter count
    }
}

TEST_CASE("Model Error Handling") {
    using namespace ZeticML;

    auto& registry = get_model_registry();
    auto model = registry.create_model("linear", 3);

    SUBCASE("Input size validation") {
        std::vector<float> wrong_input = {1.0f, 2.0f}; // Wrong size
        CHECK_THROWS(model->forward(wrong_input));
    }

    SUBCASE("Parameter size validation") {
        std::vector<float> wrong_params = {1.0f, 2.0f}; // Wrong size
        CHECK_THROWS(model->set_parameters(wrong_params));
    }
}