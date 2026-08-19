export enum WaveType {
  SINE = 0,
  SQUARE = 1,
  SAW = 2,
}

export enum EnvelopeStage {
  STAGE_OFF = 0,
  STAGE_ATTACK = 1,
  STAGE_DECAY = 2,
  STAGE_SUSTAIN = 3,
  STAGE_RELEASE = 4,
}

export interface ADSRParams {
  attack: number; // in seconds
  decay: number; // in seconds
  sustain: number; // 0.0 - 1.0
  release: number; // in seconds
}

export interface KeyMapEntry {
  key: string;
  note: number;
  label: string;
  name: string;
}

export interface VoiceState {
  id: number;
  active: boolean;
  midiKey: number;
  frequency: number;
  envelopeLevel: number;
}
