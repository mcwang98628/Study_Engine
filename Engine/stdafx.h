#pragma once

// Standard C++ includes
#include <array>
#include <vector>
#include <optional>
#include <cstddef>  // for offsetof
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <set>
#include <unordered_map>
#include <string>
#include <fstream>
#include <chrono>
#include <memory>


#define MOUSE_SENSITIVITY 0.1f
#define ENABLE_MOUSE_CALLBACK true

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define MAX_FRAMES_IN_FLIGHT 2

#define WIDTH 2560.f
#define HEIGHT 1440.f

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef STUDY_ENGINE_EXPORTS
#define STUDY_ENGINE __declspec(dllexport)
#else
#define STUDY_ENGINE __declspec(dllimport)
#endif

