/**
 * ZeticML Assignment - Two-Layer MLP Implementation
 * Separate implementation following assignment requirements
 */

#include "two_layer_mlp.h"
#include <stdexcept>
#include <algorithm>

namespace ZeticML {

TwoLayerMLP::TwoLayerMLP(size_t input_size, size_t hidden_size, size_t output_size)
    : input_size_(input_size), hidden_size_(hidden_size), output_size_(output_size) {

    W1_.resize(input_size_ * hidden_size_, 0.0f);
    b1_.resize(hidden_size_, 0.0f);
    W2_.resize(hidden_size_ * output_size_, 0.0f);
    b2_.resize(output_size_, 0.0f);
}

std::vector<float> TwoLayerMLP::forward(const std::vector<float>& input) {
    if (input.size() != input_size_) {
        throw std::invalid_argument("Input size mismatch");
    }

    // Hidden layer forward pass
    std::vector<float> hidden(hidden_size_, 0.0f);
    for (size_t h = 0; h < hidden_size_; ++h) {
        hidden[h] = b1_[h];
        for (size_t i = 0; i < input_size_; ++i) {
            hidden[h] += W1_[i * hidden_size_ + h] * input[i];
        }
        // ReLU activation
        hidden[h] = std::max(0.0f, hidden[h]);
    }

    // Output layer forward pass
    std::vector<float> output(output_size_, 0.0f);
    for (size_t o = 0; o < output_size_; ++o) {
        output[o] = b2_[o];
        for (size_t h = 0; h < hidden_size_; ++h) {
            output[o] += W2_[h * output_size_ + o] * hidden[h];
        }
    }

    return output;
}

void TwoLayerMLP::set_parameters(const std::vector<float>& parameters) {
    size_t expected_size = W1_.size() + b1_.size() + W2_.size() + b2_.size();
    if (parameters.size() != expected_size) {
        throw std::invalid_argument("Parameter size mismatch");
    }

    size_t idx = 0;

    // Copy parameters in order: W1, b1, W2, b2
    for (size_t i = 0; i < W1_.size(); ++i) {
        W1_[i] = parameters[idx++];
    }
    for (size_t i = 0; i < b1_.size(); ++i) {
        b1_[i] = parameters[idx++];
    }
    for (size_t i = 0; i < W2_.size(); ++i) {
        W2_[i] = parameters[idx++];
    }
    for (size_t i = 0; i < b2_.size(); ++i) {
        b2_[i] = parameters[idx++];
    }
}

size_t TwoLayerMLP::input_size() const {
    return input_size_;
}

size_t TwoLayerMLP::output_size() const {
    return output_size_;
}

std::string TwoLayerMLP::get_model_type() const {
    return "Two-Layer MLP";
}


} // namespace ZeticML