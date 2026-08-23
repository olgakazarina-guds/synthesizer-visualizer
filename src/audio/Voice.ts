// ==============================================================================
// Voice.ts
// Polyphonic Voice Component (Composition: has an Envelope and dedicated Oscillator).
//
// Role in project architecture:
// - Represents one active or idle musical voice.
// - Plays a given MIDI frequency and multiplies the oscillator sample by the envelope.
// ==============================================================================

import { WaveType } from '../types';
import { Oscillator, SineOscillator, SquareOscillator, SawOscillator } from './Oscillator';
import { Envelope } from './Envelope';

export class Voice {
  private sampleRate: number;
  private oscillator: Oscillator | null = null;
  private currentWaveType: WaveType = WaveType.SINE;
  private envelope: Envelope;
  private currentMidiKey: number = -1;
  private currentFrequency: number = 440.0;
  private noteActive: boolean = false;

  constructor(sampleRate: number = 44100) {
    this.sampleRate = sampleRate;
    this.envelope = new Envelope(sampleRate);
    this.setWaveType(WaveType.SINE);
  }

  // Allocate dedicated oscillator for this voice (Sine, Square, Saw)
  setWaveType(type: WaveType): void {
    this.currentWaveType = type;
    switch (type) {
      case WaveType.SINE:
        this.oscillator = new SineOscillator(this.sampleRate);
        break;
      case WaveType.SQUARE:
        this.oscillator = new SquareOscillator(this.sampleRate);
        break;
      case WaveType.SAW:
        this.oscillator = new SawOscillator(this.sampleRate);
        break;
    }
    if (this.oscillator !== null) {
      this.oscillator.setFrequency(this.currentFrequency);
    }
  }

  // Start note playback
  playNote(midiKey: number, frequency: number): void {
    this.currentMidiKey = midiKey;
    this.currentFrequency = frequency;
    this.noteActive = true;

    if (this.oscillator !== null) {
      this.oscillator.setFrequency(frequency);
    }
    this.envelope.triggerAttack();
  }

  // Stop note playback
  stopNote(): void {
    this.noteActive = false;
    this.envelope.triggerRelease();
  }

  // Output product of oscillator sample and envelope volume
  generateSample(): number {
    if (!this.envelope.isActive() || this.oscillator === null) {
      return 0.0;
    }

    const rawSample = this.oscillator.generateSample();
    const envVolume = this.envelope.process();

    return rawSample * envVolume;
  }

  isPlaying(): boolean {
    return this.envelope.isActive();
  }

  getNoteKey(): number {
    return this.currentMidiKey;
  }

  getFrequency(): number {
    return this.currentFrequency;
  }

  getWaveType(): WaveType {
    return this.currentWaveType;
  }

  getEnvelope(): Envelope {
    return this.envelope;
  }

  isNoteActive(): boolean {
    return this.noteActive;
  }
}

