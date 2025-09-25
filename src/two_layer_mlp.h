/**
 * ZeticML Assignment - Two-Layer MLP Implementation Header
 * Separate implementation following assignment requirements
 */

#pragma once

#include "neural_network_interface.h"
#include <vector>

namespace ZeticML {

/**
 * Two-Layer MLP: Multi-Layer Perceptron with one hidden layer
 * Hidden layer uses ReLU activation, output layer is linear
 * Demonstrates more complex neural network architecture
 */
class TwoLayerMLP : public NeuralNetwork {
private:
    std::vector<float> W1_;        // Input to hidden weights [input_size * hidden_size]
    std::vector<float> b1_;        // Hidden layer biases [hidden_size]
    std::vector<float> W2_;        // Hidden to output weights [hidden_size * output_size]
    std::vector<float> b2_;        // Output layer biases [output_size]
    size_t input_size_;
    size_t hidden_size_;
    size_t output_size_;

public:
    TwoLayerMLP(size_t input_size, size_t hidden_size, size_t output_size);

    // Implementation of NeuralNetwork interface
    std::vector<float> forward(const std::vector<float>& input) override;
    void set_parameters(const std::vector<float>& parameters) override;
    size_t input_size() const override;
    size_t output_size() const override;
    std::string get_model_type() const override;
};


} // namespace ZeticML