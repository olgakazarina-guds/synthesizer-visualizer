// ==============================================================================
// Oscillator.ts
// Abstract Base Class and Concrete Waveform Oscillators (Inheritance & Polymorphism).
//
// Role in project architecture:
// - Defines the abstract Oscillator base class with common fields (frequency, sampleRate, phase).
// - Subclasses implement generateSample() with their specific DSP formula.
// ==============================================================================

// Base Abstract Oscillator Class (Inheritance: IS-A)
export abstract class Oscillator {
  protected frequency: number = 440.0;
  protected sampleRate: number = 44100;
  protected phase: number = 0.0;

  constructor(sr: number = 44100) {
    this.sampleRate = sr;
  }

  // Update pitch in Hertz
  setFrequency(freq: number): void {
    this.frequency = freq;
  }

  // Update sample rate
  setSampleRate(rate: number): void {
    this.sampleRate = rate;
  }

  // Abstract method implemented by each waveform subclass
  abstract generateSample(): number;
}

// 1. Concrete Sine Wave Oscillator
export class SineOscillator extends Oscillator {
  constructor(sr: number = 44100) {
    super(sr);
  }

  // Uses trigonometric sine: sample = sin(2 * pi * phase)
  generateSample(): number {
    const sample = Math.sin(this.phase * 2.0 * Math.PI);
    this.phase += this.frequency / this.sampleRate;
    if (this.phase >= 1.0) this.phase -= 1.0;
    return sample;
  }
}

// 2. Concrete Square Wave Oscillator
export class SquareOscillator extends Oscillator {
  constructor(sr: number = 44100) {
    super(sr);
  }

  // Duty cycle thresholding: +0.8 when phase < 0.5, else -0.8
  generateSample(): number {
    const sample = this.phase < 0.5 ? 0.8 : -0.8;
    this.phase += this.frequency / this.sampleRate;
    if (this.phase >= 1.0) this.phase -= 1.0;
    return sample;
  }
}

// 3. Concrete Sawtooth Wave Oscillator
export class SawOscillator extends Oscillator {
  constructor(sr: number = 44100) {
    super(sr);
  }

  // Linear ramp formula: 2.0 * phase - 1.0 (scaled by 0.7)
  generateSample(): number {
    const sample = 2.0 * this.phase - 1.0;
    this.phase += this.frequency / this.sampleRate;
    if (this.phase >= 1.0) this.phase -= 1.0;
    return sample * 0.7;
  }
}

