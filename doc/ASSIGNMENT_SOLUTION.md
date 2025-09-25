# ZeticML Neural Network Interface Framework - Assignment Solution

## Assignment Solution Overview

This implements a **modular neural network inference framework** based on polymorphism, providing a clean interface that allows different neural network implementations to be used interchangeably through a common base class.

## Core Interface Design

### 1. Abstract Neural Network Interface
```cpp
namespace ZeticML {
    class NeuralNetwork {
    public:
        virtual ~NeuralNetwork() = default;

        // Core inference function
        virtual std::vector<float> forward(const std::vector<float>& input) = 0;

        // Set model parameters (weights, biases)
        virtual void set_parameters(const std::vector<float>& parameters) = 0;

        // Model metadata
        virtual size_t input_size() const = 0;
        virtual size_t output_size() const = 0;
        virtual std::string get_model_type() const = 0;
    };
}
```

### 2. Polymorphic Object Creation via Registry Pattern
```cpp
// Get the global model registry
auto& registry = get_model_registry();

// Create models polymorphically using method overloads
auto linear_model = registry.create_model("linear", input_size);
auto logistic_model = registry.create_model("logistic", input_size);
auto classifier = registry.create_model("multiclass", input_size, num_classes);
auto mlp = registry.create_model("mlp", input_size, hidden_size, output_size);
```

## Implemented Neural Network Types

### 1. Linear Regression
```cpp
class LinearRegression : public NeuralNetwork {
    // Implementation: output = w1*x1 + w2*x2 + ... + bias
    // Parameters: [w1, w2, ..., wn, bias]
    // Input/Output: Any size as specified during construction
};
```

### 2. Logistic Regression
```cpp
class LogisticRegression : public NeuralNetwork {
    // Implementation: output = sigmoid(w1*x1 + w2*x2 + ... + bias)
    // Parameters: [w1, w2, ..., wn, bias]
    // Output: Single probability value [0, 1]
};
```

### 3. Multi-Class Classifier
```cpp
class MultiClassClassifier : public NeuralNetwork {
    // Implementation: Softmax over multiple linear outputs
    // Parameters: [class0_weights..., class0_bias, class1_weights..., class1_bias, ...]
    // Output: Probability distribution over classes
};
```

### 4. Two-Layer MLP
```cpp
class TwoLayerMLP : public NeuralNetwork {
    // Implementation: Hidden layer with ReLU, linear output layer
    // Parameters: [W1_weights..., b1_biases..., W2_weights..., b2_biases...]
    // Architecture: input -> ReLU(W1*x + b1) -> W2*h + b2 -> output
};
```

## Concrete Example Usage

### Basic Usage Pattern
```cpp
#include "neural_network_interface.h"
#include "model_registry.h"

// Get the global model registry
auto& registry = get_model_registry();

// Create a model polymorphically using registry
auto model = registry.create_model("linear", 3);  // 3 input features

// Set parameters (weights + bias)
std::vector<float> params = {0.5f, 0.3f, 0.2f, 0.1f};  // w1, w2, w3, bias
model->set_parameters(params);

// Run inference
std::vector<float> input = {1.0f, 2.0f, -0.5f};
std::vector<float> output = model->forward(input);

std::cout << "Linear Regression Output: " << output[0] << std::endl;
// Expected: 0.5*1.0 + 0.3*2.0 + 0.2*(-0.5) + 0.1 = 1.0
```

### Configuration-Driven Polymorphic Usage
```cpp
// Configuration structure - could come from JSON file, database, etc.
struct ModelConfig {
    std::string type;
    std::vector<size_t> dimensions;
    std::vector<float> parameters;
};

// Load configurations (simulated - could be from external source)
std::vector<ModelConfig> configurations = {
    {"linear", {2}, {0.7f, 0.3f, 0.0f}},
    {"logistic", {2}, {0.8f, -0.4f, 0.1f}},
    {"multiclass", {2, 3}, {0.5f, 0.3f, 0.1f, -0.2f, 0.6f, -0.1f, 0.1f, -0.4f, 0.2f}},
    {"mlp", {2, 4, 2}, std::vector<float>(22, 0.2f)}
};

auto& registry = get_model_registry();
std::vector<float> test_input = {0.5f, -0.2f};

// Pure polymorphic loop - same code processes ALL model types
for (const auto& config : configurations) {
    // Create model polymorphically from string configuration
    std::unique_ptr<NeuralNetwork> model;
    if (config.dimensions.size() == 1) {
        model = registry.create_model(config.type, config.dimensions[0]);
    } else if (config.dimensions.size() == 2) {
        model = registry.create_model(config.type, config.dimensions[0], config.dimensions[1]);
    } else if (config.dimensions.size() == 3) {
        model = registry.create_model(config.type, config.dimensions[0], config.dimensions[1], config.dimensions[2]);
    }

    // All interactions through base class interface - true polymorphism!
    std::cout << "Model: " << model->get_model_type() << std::endl;
    model->set_parameters(config.parameters);
    auto result = model->forward(test_input);
    // Process result...
}
```

## Sample Input/Output Examples

### 1. Linear Regression Example
```
Input:  [1.5, -0.5, 2.0] (3 features)
Params: [0.5, 0.3, 0.2, 0.1] (weights + bias)
Computation: 0.5*1.5 + 0.3*(-0.5) + 0.2*2.0 + 0.1 = 1.0
Output: [1.0]
```

### 2. Logistic Regression Example
```
Input:  [0.8, -0.3] (2 features)
Params: [1.2, -0.8, 0.5] (weights + bias)
Computation: sigmoid(1.2*0.8 + (-0.8)*(-0.3) + 0.5) = sigmoid(1.54) = 0.823
Output: [0.823]
```

### 3. Multi-Class Classifier Example
```
Input:  [0.6, -0.4] (2 features)
Classes: 3
Params: [1.0, 0.5, 0.2,    // class 0: weights + bias
         -0.5, 1.2, -0.1,  // class 1: weights + bias
         0.2, -0.8, 0.3]   // class 2: weights + bias
Computation: Softmax over [0.7, 0.58, -0.62]
Output: [0.425, 0.380, 0.195] (probabilities sum to 1.0)
```

### 4. Two-Layer MLP Example
```
Input:   [1.5, -0.8] (2 inputs)
Hidden:  4 neurons
Output:  2 neurons
Params:  17 values total (W1: 8, b1: 4, W2: 8, b2: 2)
Forward: input -> ReLU(W1*input + b1) -> W2*hidden + b2 -> output
Output:  [result1, result2]
```

## Key Design Features

### 1. Polymorphic Interface Design
- **Abstract base class (`NeuralNetwork`)** defines common interface
- **Virtual functions** enable runtime polymorphism
- All implementations inherit from the same base class
- Uniform interface allows different neural network types to be used interchangeably

### 2. Registry Pattern for Runtime Creation
- **Method overloading** handles different constructor signatures elegantly
- **String-based type selection** enables configuration-driven model creation
- **Runtime type resolution** - model types determined at execution time
- **Type safety** through C++ method overloads rather than template complexity

### 3. Modular Architecture
- Each implementation in separate header/source files (`src/linear_regression.h/.cpp`, etc.)
- Clean separation of interface definition from implementations
- Easy to add new neural network types without modifying existing code

## Framework Benefits

1. **Runtime Polymorphism**: Model types selected at runtime from string configurations
2. **Swappable Implementations**: Easy to switch between different neural network types
3. **Type Safety**: C++ method overloads provide compile-time parameter checking
4. **Configuration-Driven**: Models can be specified in config files, user input, or databases
5. **Extensible**: Framework supports adding new model types without changing client code
6. **Clean Interface**: Common methods across all implementations enable uniform usage

## Polymorphism Demonstration

This framework demonstrates **three levels of polymorphism**:

### 1. Interface Polymorphism
All models implement the same `NeuralNetwork` interface with virtual functions, enabling uniform usage regardless of concrete type.

### 2. Creation Polymorphism
The registry uses **C++ method overloads** to handle different constructor signatures elegantly:
```cpp
registry.create_model("linear", 3);           // Calls LinearRegression(3)
registry.create_model("multiclass", 2, 3);   // Calls MultiClassClassifier(2, 3)
registry.create_model("mlp", 2, 4, 2);      // Calls TwoLayerMLP(2, 4, 2)
```

### 3. Runtime Polymorphism
Model types are resolved at runtime from string names, enabling true configuration-driven architecture where the same code works with any registered model type.

This demonstrates a **neural network inference framework** built on comprehensive polymorphic design principles, showcasing how C++ polymorphism can create flexible, maintainable, and extensible software architectures.

## Cross-Platform Support

### Desktop and Mobile Deployment

The framework is designed for **cross-platform deployment** supporting both desktop and mobile environments:

#### **Desktop Platforms** ✅
- **Linux/WSL**: Full CMake build support
- **Windows**: Native compilation with MSVC or MinGW
- **macOS**: Standard Unix-style build process

#### **Android NDK Integration** 🚀
The framework seamlessly integrates with **Android NDK** for mobile deployment:

```bash
# Android NDK build using existing CMake
./scripts/build_android_tests.sh

# Leverages existing CMakeLists.txt with Android toolchain
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-21
```

**Key Android Features:**
- ✅ **Native C++ execution** through Android NDK
- ✅ **Same codebase** runs on desktop and Android
- ✅ **doctest unit tests** execute directly on Android devices via ADB
- ✅ **Registry pattern** enables JNI integration for string-based model selection
- ✅ **No code modifications** required - existing framework works unchanged
- ✅ **Production ready** for Zetic.ai mobile deployment scenarios

### Mobile Architecture Benefits

The **registry pattern** is particularly well-suited for mobile deployment:

1. **JNI Integration**: Java/Kotlin code can create models via string names:
   ```java
   // Java side
   String modelType = "linear";
   float[] result = ZeticML.runInference(modelType, inputData);

   // Maps to C++ registry call
   auto model = registry.create_model("linear", input_size);
   ```

2. **Runtime Model Selection**: Perfect for mobile apps that need to switch between different ML models based on user preferences or device capabilities

3. **Memory Efficiency**: Native C++ execution provides optimal performance on mobile devices

4. **Cross-Platform Testing**: Same test suite validates functionality on both desktop development environment and target mobile devices

This cross-platform capability demonstrates how the polymorphic framework design scales from research/development environments to production mobile deployments, making it suitable for real-world ML inference applications.