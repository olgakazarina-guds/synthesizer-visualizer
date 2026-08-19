#pragma once

// ==============================================================================
// WaveType.h
// Enumeration defining the available synthesizer waveforms.
//
// This enum gives friendly names to the waveform selections:
// 0: Sine wave (smooth, pure fundamental tone)
// 1: Square wave (hollow, rich in odd harmonics)
// 2: Sawtooth wave (bright, sharp, rich in all harmonics)
// ==============================================================================

enum class WaveType {
    SINE = 0,
    SQUARE = 1,
    SAW = 2
};

