#pragma once
#include <memory>

class Oscillator;   // forward declaration

enum class WaveType { SINE, SQUARE, SAW };

// Factory: creates the right oscillator for the given wave type.
std::unique_ptr<Oscillator> makeOscillator(WaveType type);
