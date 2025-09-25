/**
 * ZeticML Assignment - Logistic Regression Implementation Header
 * Separate implementation following assignment requirements
 */

#pragma once

#include "neural_network_interface.h"
#include <vector>

namespace ZeticML {

/**
 * Logistic Regression: output = sigmoid(w1*x1 + w2*x2 + ... + bias)
 * Binary classification with sigmoid activation
 */
class LogisticRegression : public NeuralNetwork {
private:
    std::vector<float> weights_;
    float bias_;
    size_t input_size_;

public:
    explicit LogisticRegression(size_t input_size);

    // Implementation of NeuralNetwork interface
    std::vector<float> forward(const std::vector<float>& input) override;
    void set_parameters(const std::vector<float>& parameters) override;
    size_t input_size() const override;
    size_t output_size() const override;
    std::string get_model_type() const override;
};


} // namespace ZeticML