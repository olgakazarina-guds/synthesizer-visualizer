// ==============================================================================
// Envelope.ts
// ADSR Volume Envelope Generator (Attack, Decay, Sustain, Release).
//
// Role in project architecture:
// - Controls note amplitude over time with 4 distinct stages to prevent audio clicks/pops.
// - process() returns a gain multiplier between 0.0 (silent) and 1.0 (full volume).
// ==============================================================================

import { EnvelopeStage } from '../types';

export class Envelope {
  private sampleRate: number;
  private currentStage: EnvelopeStage;
  private currentLevel: number;
  private releaseStartLevel: number = 0.0;
  private attackTime: number;
  private decayTime: number;
  private sustainLevel: number;
  private releaseTime: number;

  constructor(sampleRate: number = 44100) {
    this.sampleRate = sampleRate;
    this.currentStage = EnvelopeStage.STAGE_OFF;
    this.currentLevel = 0.0;
    this.releaseStartLevel = 0.0;
    this.attackTime = 0.05;
    this.decayTime = 0.1;
    this.sustainLevel = 0.7;
    this.releaseTime = 0.3;
  }

  // Update sample rate
  setSampleRate(sr: number): void {
    this.sampleRate = sr;
  }

  // Configure ADSR parameters with protective minimum values
  setADSR(a: number, d: number, s: number, r: number): void {
    this.attackTime = Math.max(0.005, a);
    this.decayTime = Math.max(0.005, d);
    this.sustainLevel = Math.max(0.0, Math.min(1.0, s));
    this.releaseTime = Math.max(0.01, r);
  }

  // Key pressed: start attack
  triggerAttack(): void {
    this.currentStage = EnvelopeStage.STAGE_ATTACK;
  }

  // Key released: start release
  triggerRelease(): void {
    if (this.currentStage !== EnvelopeStage.STAGE_OFF) {
      this.releaseStartLevel = this.currentLevel;
      if (this.releaseStartLevel <= 0.0001) {
        this.currentLevel = 0.0;
        this.currentStage = EnvelopeStage.STAGE_OFF;
      } else {
        this.currentStage = EnvelopeStage.STAGE_RELEASE;
      }
    }
  }

  // Calculate next envelope volume level
  process(): number {
    switch (this.currentStage) {
      case EnvelopeStage.STAGE_OFF:
        this.currentLevel = 0.0;
        break;

      case EnvelopeStage.STAGE_ATTACK: {
        const attackStep = 1.0 / (this.attackTime * this.sampleRate);
        this.currentLevel += attackStep;
        if (this.currentLevel >= 1.0) {
          this.currentLevel = 1.0;
          this.currentStage = EnvelopeStage.STAGE_DECAY;
        }
        break;
      }

      case EnvelopeStage.STAGE_DECAY: {
        const decayStep = (1.0 - this.sustainLevel) / (this.decayTime * this.sampleRate);
        this.currentLevel -= decayStep;
        if (this.currentLevel <= this.sustainLevel) {
          this.currentLevel = this.sustainLevel;
          this.currentStage = EnvelopeStage.STAGE_SUSTAIN;
        }
        break;
      }

      case EnvelopeStage.STAGE_SUSTAIN:
        this.currentLevel = this.sustainLevel;
        break;

      case EnvelopeStage.STAGE_RELEASE: {
        let releaseStep = this.releaseStartLevel / (this.releaseTime * this.sampleRate);
        if (releaseStep <= 0.0) releaseStep = 0.001;
        this.currentLevel -= releaseStep;
        if (this.currentLevel <= 0.0001) {
          this.currentLevel = 0.0;
          this.currentStage = EnvelopeStage.STAGE_OFF;
        }
        break;
      }
    }
    return this.currentLevel;
  }

  isActive(): boolean {
    return this.currentStage !== EnvelopeStage.STAGE_OFF;
  }

  getCurrentLevel(): number {
    return this.currentLevel;
  }

  getStage(): EnvelopeStage {
    return this.currentStage;
  }
}

