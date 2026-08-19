#include "SawOscillator.h"

float SawOscillator::waveformAt(float phase) const {
    return (2.0f * phase) - 1.0f;
}
