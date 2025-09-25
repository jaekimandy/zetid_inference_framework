/**
 * ZeticML Assignment - Linear Regression Implementation
 * Separate implementation following assignment requirements
 */

#include "linear_regression.h"
#include <stdexcept>

namespace ZeticML {

LinearRegression::LinearRegression(size_t input_size)
    : bias_(0.0f), input_size_(input_size) {
    weights_.resize(input_size, 0.0f);
}

std::vector<float> LinearRegression::forward(const std::vector<float>& input) {
    if (input.size() != input_size_) {
        throw std::invalid_argument("Input size mismatch");
    }

    float result = bias_;
    for (size_t i = 0; i < input_size_; ++i) {
        result += weights_[i] * input[i];
    }

    return {result};
}

void LinearRegression::set_parameters(const std::vector<float>& parameters) {
    if (parameters.size() != input_size_ + 1) {
        throw std::invalid_argument("Parameter size mismatch");
    }

    for (size_t i = 0; i < input_size_; ++i) {
        weights_[i] = parameters[i];
    }
    bias_ = parameters[input_size_];
}

size_t LinearRegression::input_size() const {
    return input_size_;
}

size_t LinearRegression::output_size() const {
    return 1;
}

std::string LinearRegression::get_model_type() const {
    return "Linear Regression";
}


} // namespace ZeticML