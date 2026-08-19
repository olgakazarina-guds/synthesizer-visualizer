// ==============================================================================
// types.ts
// Shared data structures and type definitions for synthesizer and UI components.
// ==============================================================================

// Waveform types matching C++ WaveType.h
export enum WaveType {
  SINE = 0,    // Pure sine wave
  SQUARE = 1,  // Square wave with odd harmonics
  SAW = 2,     // Sawtooth wave with rich harmonics
}

// ADSR envelope stage state machine
export enum EnvelopeStage {
  STAGE_OFF = 0,     // Silent
  STAGE_ATTACK = 1,  // Rising to peak
  STAGE_DECAY = 2,   // Dropping to sustain
  STAGE_SUSTAIN = 3, // Holding constant
  STAGE_RELEASE = 4, // Fading to silent
}

// ADSR timing parameters
export interface ADSRParams {
  attack: number;  // Attack duration in seconds
  decay: number;   // Decay duration in seconds
  sustain: number; // Sustain volume level (0.0 to 1.0)
  release: number; // Release duration in seconds
}

// Key mapping for computer keyboard notes
export interface KeyMapEntry {
  key: string;
  note: number;
  label: string;
  name: string;
}

// State snapshot of an individual voice for UI meters
export interface VoiceState {
  id: number;
  active: boolean;
  midiKey: number;
  frequency: number;
  envelopeLevel: number;
}

