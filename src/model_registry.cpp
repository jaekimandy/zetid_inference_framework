/**
 * ZeticML Assignment - Model Registry Implementation
 * Registry functionality is now implemented in the header file
 * This file exists to ensure the registry object file is linked
 */

#include "model_registry.h"

namespace ZeticML {

// This ensures the model_registry compilation unit gets linked
// The actual registry logic is now in the header for inline initialization
void __ensure_registry_linking() {
    // This function does nothing but ensures this object file is linked
}

} // namespace ZeticML