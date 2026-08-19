// Base Abstract Oscillator Class (Inheritance: IS-A)
export abstract class Oscillator {
  protected frequency: number = 440.0;
  protected sampleRate: number = 44100;
  protected phase: number = 0.0;

  constructor(sr: number = 44100) {
    this.sampleRate = sr;
  }

  setFrequency(freq: number): void {
    this.frequency = freq;
  }

  setSampleRate(rate: number): void {
    this.sampleRate = rate;
  }

  abstract generateSample(): number;
}

// 1. Concrete Sine Wave Oscillator
export class SineOscillator extends Oscillator {
  constructor(sr: number = 44100) {
    super(sr);
  }

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

  generateSample(): number {
    const sample = 2.0 * this.phase - 1.0;
    this.phase += this.frequency / this.sampleRate;
    if (this.phase >= 1.0) this.phase -= 1.0;
    return sample * 0.7;
  }
}
