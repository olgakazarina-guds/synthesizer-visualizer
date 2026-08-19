import { EnvelopeStage } from '../types';

export class Envelope {
  private sampleRate: number;
  private currentStage: EnvelopeStage;
  private currentLevel: number;
  private attackTime: number;
  private decayTime: number;
  private sustainLevel: number;
  private releaseTime: number;

  constructor(sampleRate: number = 44100) {
    this.sampleRate = sampleRate;
    this.currentStage = EnvelopeStage.STAGE_OFF;
    this.currentLevel = 0.0;
    this.attackTime = 0.05;
    this.decayTime = 0.1;
    this.sustainLevel = 0.7;
    this.releaseTime = 0.3;
  }

  setSampleRate(sr: number): void {
    this.sampleRate = sr;
  }

  setADSR(a: number, d: number, s: number, r: number): void {
    this.attackTime = Math.max(0.005, a);
    this.decayTime = Math.max(0.005, d);
    this.sustainLevel = Math.max(0.0, Math.min(1.0, s));
    this.releaseTime = Math.max(0.01, r);
  }

  triggerAttack(): void {
    this.currentStage = EnvelopeStage.STAGE_ATTACK;
  }

  triggerRelease(): void {
    if (this.currentStage !== EnvelopeStage.STAGE_OFF) {
      this.currentStage = EnvelopeStage.STAGE_RELEASE;
    }
  }

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
        const releaseStep = this.sustainLevel / (this.releaseTime * this.sampleRate);
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
