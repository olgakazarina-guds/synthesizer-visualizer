#include "SineOscillator.h"

float SineOscillator::waveformAt(float phase) const {
    return sin(phase * TWO_PI);
}
