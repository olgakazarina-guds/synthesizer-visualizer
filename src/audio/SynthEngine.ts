// ==============================================================================
// SynthEngine.ts
// Synthesizer Engine for Web Audio preview mirroring C++ openFrameworks Synth class.
//
// Role in project architecture (Olga / Left UML):
// - Manages 8 polyphonic voices and concrete oscillators.
// - Bridges sample-by-sample DSP generation to the browser Web Audio API via ScriptProcessorNode.
// - Mirrors the exact audioOut() logic of the C++ openFrameworks implementation.
// ==============================================================================

import { WaveType, VoiceState } from '../types';
import { Voice } from './Voice';

export class SynthEngine {
  private sampleRate: number = 44100;
  private bufferSize: number = 512;
  private masterVolume: number = 0.8;
  private currentWaveType: WaveType = WaveType.SINE;

  private voices: Voice[] = [];
  private monoBuffer: Float32Array;

  // Web Audio Context
  private audioCtx: AudioContext | null = null;
  private scriptNode: ScriptProcessorNode | null = null;
  private isAudioStarted: boolean = false;

  constructor(sampleRate: number = 44100, bufferSize: number = 512, polyphony: number = 8) {
    this.sampleRate = sampleRate;
    this.bufferSize = bufferSize;
    this.monoBuffer = new Float32Array(this.bufferSize);

    this.setup(sampleRate, bufferSize, polyphony);
  }

  // Initialize engine state and polyphonic voice pool
  public setup(sr: number = 44100, bufSize: number = 512, polyphony: number = 8): void {
    this.sampleRate = sr;
    this.bufferSize = bufSize;
    this.monoBuffer = new Float32Array(this.bufferSize);

    // Initialize Voice instances, each with its dedicated oscillator
    this.voices = [];
    for (let i = 0; i < polyphony; i++) {
      const v = new Voice(this.sampleRate);
      v.setWaveType(this.currentWaveType);
      this.voices.push(v);
    }
  }

  // Starts the browser audio output engine on user interaction
  public async initAudio(): Promise<boolean> {
    if (this.audioCtx && this.audioCtx.state === 'running') {
      return true;
    }

    try {
      const AudioCtxClass = window.AudioContext || (window as unknown as { webkitAudioContext: typeof AudioContext }).webkitAudioContext;
      this.audioCtx = new AudioCtxClass();
      this.sampleRate = this.audioCtx.sampleRate;

      // Re-setup with the hardware sample rate
      this.setup(this.sampleRate, this.bufferSize, 8);

      // Create ScriptProcessor for sample-exact DSP synthesis mirroring C++ audioOut()
      this.scriptNode = this.audioCtx.createScriptProcessor(this.bufferSize, 0, 2);
      this.scriptNode.onaudioprocess = (audioProcessingEvent) => {
        const outputBuffer = audioProcessingEvent.outputBuffer;
        const leftChannel = outputBuffer.getChannelData(0);
        const rightChannel = outputBuffer.getChannelData(1);

        this.processAudio(leftChannel, rightChannel);
      };

      this.scriptNode.connect(this.audioCtx.destination);
      if (this.audioCtx.state === 'suspended') {
        await this.audioCtx.resume();
      }

      this.isAudioStarted = true;
      return true;
    } catch (e) {
      console.error('Failed to initialize Web Audio:', e);
      return false;
    }
  }

  // Switch the waveform generator used by all voices
  public setWaveType(type: WaveType): void {
    this.currentWaveType = type;

    // Update all voices
    for (const voice of this.voices) {
      voice.setWaveType(type);
    }
  }

  public getWaveType(): WaveType {
    return this.currentWaveType;
  }

  // Update ADSR parameters across all voices
  public setADSR(a: number, d: number, s: number, r: number): void {
    for (const voice of this.voices) {
      voice.getEnvelope().setADSR(a, d, s, r);
    }
  }

  public setMasterVolume(vol: number): void {
    this.masterVolume = Math.max(0.0, Math.min(1.0, vol));
  }

  public getMasterVolume(): number {
    return this.masterVolume;
  }

  // Convert MIDI note number to pitch frequency in Hertz (A4 = 69 = 440 Hz)
  public static midiToFreq(midiNote: number): number {
    return 440.0 * Math.pow(2.0, (midiNote - 69) / 12.0);
  }

  // Allocate an available voice to play a note
  public noteOn(noteKey: number, frequency?: number): void {
    const freq = frequency ?? SynthEngine.midiToFreq(noteKey);

    // Auto initialize audio on first interaction if not yet started
    if (!this.isAudioStarted && this.audioCtx && this.audioCtx.state === 'suspended') {
      this.audioCtx.resume();
    }

    // Find an idle voice or steal oldest
    let targetVoice: Voice | null = null;
    for (const voice of this.voices) {
      if (!voice.isPlaying()) {
        targetVoice = voice;
        break;
      }
    }

    if (targetVoice === null && this.voices.length > 0) {
      targetVoice = this.voices[0]; // fallback voice stealing
    }

    if (targetVoice !== null) {
      targetVoice.playNote(noteKey, freq);
    }
  }

  // Stop playing a note
  public noteOff(noteKey: number): void {
    for (const voice of this.voices) {
      if (voice.isPlaying() && voice.getNoteKey() == noteKey) {
        voice.stopNote();
      }
    }
  }

  public stopAllNotes(): void {
    for (const voice of this.voices) {
      if (voice.isPlaying()) {
        voice.stopNote();
      }
    }
  }

  // DSP processing loop: sum active voices and write to stereo channels
  public processAudio(leftChannel: Float32Array, rightChannel: Float32Array): void {
    const numFrames = leftChannel.length;
    if (this.monoBuffer.length !== numFrames) {
      this.monoBuffer = new Float32Array(numFrames);
    }

    for (let i = 0; i < numFrames; i++) {
      let mixedSample = 0.0;

      // Sum active voices
      for (const voice of this.voices) {
        if (voice.isPlaying()) {
          mixedSample += voice.generateSample();
        }
      }

      // Master volume and clamp to [-1.0, 1.0]
      mixedSample *= this.masterVolume;
      mixedSample = Math.max(-1.0, Math.min(1.0, mixedSample));

      // Store in monoBuffer for visualizer
      this.monoBuffer[i] = mixedSample;

      // Stereo output
      leftChannel[i] = mixedSample;
      rightChannel[i] = mixedSample;
    }
  }

  public getBuffer(): Float32Array {
    return this.monoBuffer;
  }

  public getVoiceStates(): VoiceState[] {
    return this.voices.map((v, i) => ({
      id: i + 1,
      active: v.isPlaying(),
      midiKey: v.getNoteKey(),
      frequency: v.getFrequency(),
      envelopeLevel: v.getEnvelope().getCurrentLevel(),
    }));
  }

  public isStarted(): boolean {
    return this.isAudioStarted;
  }
}

