/**
 * ZeticML Assignment - Neural Network Interface
 * Clean, swappable interface for neural network inference
 */

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <memory>

namespace ZeticML {

/**
 * Abstract neural network interface for swappable implementations
 * Provides clean numeric input/output with consistent API
 *
 * Design choices:
 * - Pure numeric interface (std::vector<float>) for simplicity
 * - Separates model loading (set_parameters) from inference (forward)
 * - Provides metadata (input/output sizes, model type)
 * - Enables polymorphic usage of different implementations
 */
class NeuralNetwork {
public:
    virtual ~NeuralNetwork() = default;

    // Core inference function - numeric input -> numeric output
    virtual std::vector<float> forward(const std::vector<float>& input) = 0;

    // Set model parameters (weights, biases)
    virtual void set_parameters(const std::vector<float>& parameters) = 0;

    // Model metadata
    virtual size_t input_size() const = 0;
    virtual size_t output_size() const = 0;
    virtual std::string get_model_type() const = 0;

    // Optional: Model information
    virtual void print_info() const {
        std::cout << "Model: " << get_model_type()
                  << " (Input: " << input_size()
                  << ", Output: " << output_size() << ")" << std::endl;
    }
};

// Factory functions are now declared in individual implementation headers

} // namespace ZeticML