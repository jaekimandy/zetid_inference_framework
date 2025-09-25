/**
 * ZeticML Assignment - Model Registry Pattern
 * Provides runtime polymorphic model creation
 */

#pragma once

#include "neural_network_interface.h"
#include "linear_regression.h"
#include "logistic_regression.h"
#include "multi_class_classifier.h"
#include "two_layer_mlp.h"
#include <unordered_map>
#include <functional>
#include <memory>
#include <vector>
#include <stdexcept>

namespace ZeticML {

/**
 * Registry pattern for polymorphic neural network creation
 * Uses method overloads for different constructor signatures
 */
class ModelRegistry {
private:
    std::unordered_map<std::string, std::string> registered_types_;

public:
    /**
     * Create model with 1 parameter (LinearRegression, LogisticRegression)
     */
    std::unique_ptr<NeuralNetwork> create_model(const std::string& type_name, size_t input_size) {
        if (type_name == "linear") {
            return std::make_unique<LinearRegression>(input_size);
        } else if (type_name == "logistic") {
            return std::make_unique<LogisticRegression>(input_size);
        } else {
            throw std::invalid_argument("Unknown single-parameter model type: " + type_name);
        }
    }

    /**
     * Create model with 2 parameters (MultiClassClassifier)
     */
    std::unique_ptr<NeuralNetwork> create_model(const std::string& type_name,
                                               size_t input_size, size_t param2) {
        if (type_name == "multiclass") {
            return std::make_unique<MultiClassClassifier>(input_size, param2);
        } else {
            throw std::invalid_argument("Unknown two-parameter model type: " + type_name);
        }
    }

    /**
     * Create model with 3 parameters (TwoLayerMLP)
     */
    std::unique_ptr<NeuralNetwork> create_model(const std::string& type_name,
                                               size_t input_size, size_t hidden_size, size_t output_size) {
        if (type_name == "mlp") {
            return std::make_unique<TwoLayerMLP>(input_size, hidden_size, output_size);
        } else {
            throw std::invalid_argument("Unknown three-parameter model type: " + type_name);
        }
    }

    /**
     * Check if a model type is registered
     */
    bool is_registered(const std::string& type_name) const {
        return type_name == "linear" || type_name == "logistic" ||
               type_name == "multiclass" || type_name == "mlp";
    }

    /**
     * Get all registered model type names
     */
    std::vector<std::string> get_registered_types() const {
        return {"linear", "logistic", "multiclass", "mlp"};
    }

    /**
     * Get the global registry instance (singleton pattern)
     */
    static ModelRegistry& instance() {
        static ModelRegistry registry;
        return registry;
    }
};

/**
 * Convenience function to get the global registry
 */
inline ModelRegistry& get_model_registry() {
    return ModelRegistry::instance();
}

} // namespace ZeticML