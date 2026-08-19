#include "WaveType.h"
#include "SineOscillator.h"
#include "SquareOscillator.h"
#include "SawOscillator.h"

std::unique_ptr<Oscillator> makeOscillator(WaveType type){
    switch (type) {
        case WaveType::SINE:   return std::make_unique<SineOscillator>();
        case WaveType::SQUARE: return std::make_unique<SquareOscillator>();
        case WaveType::SAW:    return std::make_unique<SawOscillator>();
    }
    return nullptr;
}
