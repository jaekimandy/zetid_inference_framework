/**
 * ZeticML Assignment - Multi-Class Classifier Implementation
 * Separate implementation following assignment requirements
 */

#include "multi_class_classifier.h"
#include <stdexcept>
#include <cmath>
#include <algorithm>

namespace ZeticML {

MultiClassClassifier::MultiClassClassifier(size_t input_size, size_t num_classes)
    : input_size_(input_size), num_classes_(num_classes) {
    weights_.resize(num_classes_, std::vector<float>(input_size_, 0.0f));
    biases_.resize(num_classes_, 0.0f);
}

std::vector<float> MultiClassClassifier::forward(const std::vector<float>& input) {
    if (input.size() != input_size_) {
        throw std::invalid_argument("Input size mismatch");
    }

    // Compute logits for each class
    std::vector<float> logits(num_classes_);
    for (size_t c = 0; c < num_classes_; ++c) {
        logits[c] = biases_[c];
        for (size_t i = 0; i < input_size_; ++i) {
            logits[c] += weights_[c][i] * input[i];
        }
    }

    // Apply softmax activation
    std::vector<float> probabilities(num_classes_);
    float max_logit = *std::max_element(logits.begin(), logits.end());

    // Subtract max for numerical stability
    float sum_exp = 0.0f;
    for (size_t c = 0; c < num_classes_; ++c) {
        probabilities[c] = std::exp(logits[c] - max_logit);
        sum_exp += probabilities[c];
    }

    // Normalize to get probabilities
    for (size_t c = 0; c < num_classes_; ++c) {
        probabilities[c] /= sum_exp;
    }

    return probabilities;
}

void MultiClassClassifier::set_parameters(const std::vector<float>& parameters) {
    size_t expected_size = num_classes_ * input_size_ + num_classes_; // weights + biases
    if (parameters.size() != expected_size) {
        throw std::invalid_argument("Parameter size mismatch");
    }

    size_t param_idx = 0;

    // Set weights
    for (size_t c = 0; c < num_classes_; ++c) {
        for (size_t i = 0; i < input_size_; ++i) {
            weights_[c][i] = parameters[param_idx++];
        }
    }

    // Set biases
    for (size_t c = 0; c < num_classes_; ++c) {
        biases_[c] = parameters[param_idx++];
    }
}

size_t MultiClassClassifier::input_size() const {
    return input_size_;
}

size_t MultiClassClassifier::output_size() const {
    return num_classes_;
}

std::string MultiClassClassifier::get_model_type() const {
    return "Multi-Class Classifier (" + std::to_string(num_classes_) + " classes)";
}


} // namespace ZeticML