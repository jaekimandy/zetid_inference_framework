/**
 * ZeticML Assignment - Multi-Class Classifier Implementation Header
 * Separate implementation following assignment requirements
 */

#pragma once

#include "neural_network_interface.h"
#include <vector>

namespace ZeticML {

/**
 * Multi-Class Classifier: Softmax-based classification for multiple classes
 * Uses linear transformations followed by softmax activation
 * Perfect for demonstrating interface flexibility with multiple outputs
 */
class MultiClassClassifier : public NeuralNetwork {
private:
    std::vector<std::vector<float>> weights_;  // [num_classes][input_size]
    std::vector<float> biases_;                // [num_classes]
    size_t input_size_;
    size_t num_classes_;

public:
    MultiClassClassifier(size_t input_size, size_t num_classes);

    // Implementation of NeuralNetwork interface
    std::vector<float> forward(const std::vector<float>& input) override;
    void set_parameters(const std::vector<float>& parameters) override;
    size_t input_size() const override;
    size_t output_size() const override;
    std::string get_model_type() const override;
};


} // namespace ZeticML