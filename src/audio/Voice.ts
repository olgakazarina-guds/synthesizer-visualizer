import { Oscillator } from './Oscillator';
import { Envelope } from './Envelope';

export class Voice {
  private oscillatorPtr: Oscillator | null = null;
  private envelope: Envelope;
  private currentMidiKey: number = -1;
  private currentFrequency: number = 440.0;
  private noteActive: boolean = false;

  constructor(sampleRate: number = 44100) {
    this.envelope = new Envelope(sampleRate);
  }

  setOscillator(osc: Oscillator | null): void {
    this.oscillatorPtr = osc;
  }

  playNote(midiKey: number, frequency: number): void {
    this.currentMidiKey = midiKey;
    this.currentFrequency = frequency;
    this.noteActive = true;

    if (this.oscillatorPtr !== null) {
      this.oscillatorPtr.setFrequency(frequency);
    }
    this.envelope.triggerAttack();
  }

  stopNote(): void {
    this.noteActive = false;
    this.envelope.triggerRelease();
  }

  generateSample(): number {
    if (!this.envelope.isActive() || this.oscillatorPtr === null) {
      return 0.0;
    }

    const rawSample = this.oscillatorPtr.generateSample();
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

  getEnvelope(): Envelope {
    return this.envelope;
  }

  isNoteActive(): boolean {
    return this.noteActive;
  }
}
