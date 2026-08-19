#include "SquareOscillator.h"

float SquareOscillator::waveformAt(float phase) const {
    return (phase < 0.5f) ? 1.0f : -1.0f;
}
