/**
 * Test Data Loader for Neural Network Testing
 * Reads input/output pairs from text files for systematic testing
 */

#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace ZeticML {

struct TestCase {
    std::vector<float> input;
    std::vector<float> expected_output;
    std::string description;

    TestCase(const std::vector<float>& in, const std::vector<float>& out, const std::string& desc = "")
        : input(in), expected_output(out), description(desc) {}
};

class TestDataLoader {
public:
    /**
     * Load test cases from file
     * File format: input1,input2,... -> expected_output1,expected_output2,...
     * Lines starting with # are comments
     */
    static std::vector<TestCase> load_from_file(const std::string& filename) {
        std::vector<TestCase> test_cases;
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Warning: Could not open test file: " << filename << std::endl;
            return test_cases;
        }

        std::string line;
        int line_number = 0;

        while (std::getline(file, line)) {
            line_number++;

            // Skip empty lines and comments
            if (line.empty() || line[0] == '#') {
                continue;
            }

            try {
                TestCase test_case = parse_line(line, line_number);
                test_cases.push_back(test_case);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing line " << line_number << " in " << filename
                         << ": " << e.what() << std::endl;
            }
        }

        std::cout << "Loaded " << test_cases.size() << " test cases from " << filename << std::endl;
        return test_cases;
    }

private:
    static TestCase parse_line(const std::string& line, int line_number) {
        // Find the arrow separator
        size_t arrow_pos = line.find("->");
        if (arrow_pos == std::string::npos) {
            throw std::runtime_error("Missing '->' separator");
        }

        // Parse input part
        std::string input_str = line.substr(0, arrow_pos);
        std::vector<float> inputs = parse_float_list(trim(input_str));

        // Parse output part
        std::string output_str = line.substr(arrow_pos + 2);
        std::vector<float> outputs = parse_float_list(trim(output_str));

        // Create description
        std::string description = "Line " + std::to_string(line_number);

        return TestCase(inputs, outputs, description);
    }

    static std::vector<float> parse_float_list(const std::string& str) {
        std::vector<float> values;
        std::stringstream ss(str);
        std::string token;

        while (std::getline(ss, token, ',')) {
            token = trim(token);
            if (!token.empty()) {
                values.push_back(std::stof(token));
            }
        }

        return values;
    }

    static std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";

        size_t end = str.find_last_not_of(" \t\r\n");
        return str.substr(start, end - start + 1);
    }
};

} // namespace ZeticML