/**
 * ZeticML Assignment - Logistic Regression Implementation
 * Separate implementation following assignment requirements
 */

#include "logistic_regression.h"
#include <stdexcept>
#include <cmath>

namespace ZeticML {

LogisticRegression::LogisticRegression(size_t input_size)
    : bias_(0.0f), input_size_(input_size) {
    weights_.resize(input_size, 0.0f);
}

std::vector<float> LogisticRegression::forward(const std::vector<float>& input) {
    if (input.size() != input_size_) {
        throw std::invalid_argument("Input size mismatch");
    }

    // Compute linear combination
    float linear_output = bias_;
    for (size_t i = 0; i < input_size_; ++i) {
        linear_output += weights_[i] * input[i];
    }

    // Apply sigmoid activation
    float sigmoid_output = 1.0f / (1.0f + std::exp(-linear_output));
    return {sigmoid_output};
}

void LogisticRegression::set_parameters(const std::vector<float>& parameters) {
    if (parameters.size() != input_size_ + 1) {
        throw std::invalid_argument("Parameter size mismatch");
    }

    for (size_t i = 0; i < input_size_; ++i) {
        weights_[i] = parameters[i];
    }
    bias_ = parameters[input_size_];
}

size_t LogisticRegression::input_size() const {
    return input_size_;
}

size_t LogisticRegression::output_size() const {
    return 1;
}

std::string LogisticRegression::get_model_type() const {
    return "Logistic Regression";
}


} // namespace ZeticML