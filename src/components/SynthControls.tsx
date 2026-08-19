// ==============================================================================
// SynthControls.tsx
// Interactive control panel for waveform selection, ADSR sliders, and voice status.
//
// Sections:
// 1. Oscillator Waveform Selector: Sine (1), Square (2), Sawtooth (3).
// 2. ADSR Envelope Controls: Attack, Decay, Sustain, Release with live SVG curve.
// 3. 8-Voice Polyphony Matrix: Real-time status indicators for all 8 voices.
// ==============================================================================

import React from 'react';
import { WaveType, ADSRParams, VoiceState } from '../types';
import { Sliders, Volume2, Waves, Zap } from 'lucide-react';

interface SynthControlsProps {
  currentWaveType: WaveType;
  onWaveTypeChange: (type: WaveType) => void;
  adsr: ADSRParams;
  onAdsrChange: (newAdsr: ADSRParams) => void;
  masterVolume: number;
  onVolumeChange: (vol: number) => void;
  voiceStates: VoiceState[];
}

export const SynthControls: React.FC<SynthControlsProps> = ({
  currentWaveType,
  onWaveTypeChange,
  adsr,
  onAdsrChange,
  masterVolume,
  onVolumeChange,
  voiceStates,
}) => {
  const waveforms = [
    { type: WaveType.SINE, label: 'Sine Wave', key: '1', desc: 'Smooth, pure tone' },
    { type: WaveType.SQUARE, label: 'Square Wave', key: '2', desc: 'Hollow, punchy harmonics' },
    { type: WaveType.SAW, label: 'Sawtooth Wave', key: '3', desc: 'Rich, bright, buzzy tone' },
  ];

  return (
    <div id="synth-controls-panel" className="grid grid-cols-1 md:grid-cols-3 gap-4">
      {/* Waveform Selection */}
      <div className="bg-[#0f1318] border border-slate-800 rounded-xl p-4 shadow-lg flex flex-col justify-between">
        <div>
          <div className="flex items-center justify-between pb-2 border-b border-slate-800 mb-3">
            <div className="flex items-center gap-2">
              <Waves className="w-4 h-4 text-emerald-400" />
              <span className="font-mono text-xs font-semibold tracking-wider text-slate-300">
                OSCILLATOR WAVEFORM (KEYS 1, 2, 3)
              </span>
            </div>
            <span className="text-[11px] font-mono text-emerald-400 font-bold">
              {waveforms.find((w) => w.type === currentWaveType)?.label}
            </span>
          </div>

          <div className="space-y-2">
            {waveforms.map((w) => {
              const isSelected = currentWaveType === w.type;
              return (
                <button
                  key={w.type}
                  id={`wave-select-btn-${w.type}`}
                  type="button"
                  onClick={() => onWaveTypeChange(w.type)}
                  className={`w-full flex items-center justify-between px-3 py-2.5 rounded-lg border text-left transition-all ${
                    isSelected
                      ? 'bg-emerald-500/10 border-emerald-500/50 text-emerald-300 shadow-[0_0_12px_rgba(16,185,129,0.15)]'
                      : 'bg-slate-900/50 border-slate-800 text-slate-400 hover:border-slate-700 hover:text-slate-200'
                  }`}
                >
                  <div className="flex items-center gap-3">
                    <span
                      className={`w-6 h-6 rounded flex items-center justify-center font-mono text-xs font-bold ${
                        isSelected
                          ? 'bg-emerald-500 text-slate-950'
                          : 'bg-slate-800 text-slate-400'
                      }`}
                    >
                      {w.key}
                    </span>
                    <div>
                      <div className="font-mono text-xs font-bold text-slate-200">
                        {w.label}
                      </div>
                      <div className="text-[10px] text-slate-400">{w.desc}</div>
                    </div>
                  </div>

                  {isSelected && (
                    <div className="w-2 h-2 rounded-full bg-emerald-400 shadow-[0_0_6px_#34d399]"></div>
                  )}
                </button>
              );
            })}
          </div>
        </div>

        {/* Master Volume */}
        <div className="mt-4 pt-3 border-t border-slate-800">
          <div className="flex items-center justify-between mb-1.5">
            <div className="flex items-center gap-1.5 text-xs font-mono text-slate-300">
              <Volume2 className="w-3.5 h-3.5 text-emerald-400" />
              <span>MASTER GAIN</span>
            </div>
            <span className="font-mono text-xs text-emerald-400 font-bold">
              {Math.round(masterVolume * 100)}%
            </span>
          </div>
          <input
            id="master-volume-slider"
            type="range"
            min="0"
            max="1"
            step="0.01"
            value={masterVolume}
            onChange={(e) => onVolumeChange(parseFloat(e.target.value))}
            className="w-full h-1.5 bg-slate-800 rounded-lg appearance-none cursor-pointer accent-emerald-400"
          />
        </div>
      </div>

      {/* ADSR Envelope Controls */}
      <div className="bg-[#0f1318] border border-slate-800 rounded-xl p-4 shadow-lg flex flex-col justify-between">
        <div>
          <div className="flex items-center justify-between pb-2 border-b border-slate-800 mb-3">
            <div className="flex items-center gap-2">
              <Sliders className="w-4 h-4 text-emerald-400" />
              <span className="font-mono text-xs font-semibold tracking-wider text-slate-300">
                ADSR VOLUME ENVELOPE
              </span>
            </div>
            <span className="text-[10px] font-mono text-slate-400">Olga / Left UML</span>
          </div>

          <div className="space-y-3">
            {/* Attack */}
            <div>
              <div className="flex justify-between text-xs font-mono mb-1">
                <span className="text-slate-300">Attack (A)</span>
                <span className="text-emerald-400">{adsr.attack.toFixed(3)} s</span>
              </div>
              <input
                id="adsr-attack-slider"
                type="range"
                min="0.005"
                max="1.5"
                step="0.005"
                value={adsr.attack}
                onChange={(e) =>
                  onAdsrChange({ ...adsr, attack: parseFloat(e.target.value) })
                }
                className="w-full h-1.5 bg-slate-800 rounded-lg appearance-none cursor-pointer accent-emerald-400"
              />
            </div>

            {/* Decay */}
            <div>
              <div className="flex justify-between text-xs font-mono mb-1">
                <span className="text-slate-300">Decay (D)</span>
                <span className="text-emerald-400">{adsr.decay.toFixed(3)} s</span>
              </div>
              <input
                id="adsr-decay-slider"
                type="range"
                min="0.005"
                max="1.5"
                step="0.005"
                value={adsr.decay}
                onChange={(e) =>
                  onAdsrChange({ ...adsr, decay: parseFloat(e.target.value) })
                }
                className="w-full h-1.5 bg-slate-800 rounded-lg appearance-none cursor-pointer accent-emerald-400"
              />
            </div>

            {/* Sustain */}
            <div>
              <div className="flex justify-between text-xs font-mono mb-1">
                <span className="text-slate-300">Sustain (S)</span>
                <span className="text-emerald-400">{Math.round(adsr.sustain * 100)}%</span>
              </div>
              <input
                id="adsr-sustain-slider"
                type="range"
                min="0.0"
                max="1.0"
                step="0.01"
                value={adsr.sustain}
                onChange={(e) =>
                  onAdsrChange({ ...adsr, sustain: parseFloat(e.target.value) })
                }
                className="w-full h-1.5 bg-slate-800 rounded-lg appearance-none cursor-pointer accent-emerald-400"
              />
            </div>

            {/* Release */}
            <div>
              <div className="flex justify-between text-xs font-mono mb-1">
                <span className="text-slate-300">Release (R)</span>
                <span className="text-emerald-400">{adsr.release.toFixed(3)} s</span>
              </div>
              <input
                id="adsr-release-slider"
                type="range"
                min="0.01"
                max="2.5"
                step="0.01"
                value={adsr.release}
                onChange={(e) =>
                  onAdsrChange({ ...adsr, release: parseFloat(e.target.value) })
                }
                className="w-full h-1.5 bg-slate-800 rounded-lg appearance-none cursor-pointer accent-emerald-400"
              />
            </div>
          </div>
        </div>

        {/* Dynamic ADSR Visual Curve */}
        <div className="mt-3 pt-2 border-t border-slate-800">
          <div className="h-10 w-full bg-slate-950/60 rounded border border-slate-800/80 p-1 relative">
            <svg className="w-full h-full" viewBox="0 0 100 30" preserveAspectRatio="none">
              <path
                d={`M 0,30 L ${Math.min(25, adsr.attack * 20)},0 L ${Math.min(
                    25 + adsr.decay * 20,
                    50
                  )},${30 - adsr.sustain * 25} L 75,${30 - adsr.sustain * 25} L ${Math.min(
                    100,
                    75 + adsr.release * 10
                  )},30`}
                fill="none"
                stroke="#34d399"
                strokeWidth="2"
              />
            </svg>
          </div>
        </div>
      </div>

      {/* Polyphonic Voice Allocation Matrix */}
      <div className="bg-[#0f1318] border border-slate-800 rounded-xl p-4 shadow-lg flex flex-col justify-between">
        <div>
          <div className="flex items-center justify-between pb-2 border-b border-slate-800 mb-3">
            <div className="flex items-center gap-2">
              <Zap className="w-4 h-4 text-emerald-400" />
              <span className="font-mono text-xs font-semibold tracking-wider text-slate-300">
                8-VOICE POLYPHONY POOL
              </span>
            </div>
            <span className="text-[10px] font-mono text-slate-400">
              Active: {voiceStates.filter((v) => v.active).length} / 8
            </span>
          </div>

          <div className="grid grid-cols-2 gap-2">
            {voiceStates.map((v) => (
              <div
                key={v.id}
                id={`voice-indicator-${v.id}`}
                className={`p-2 rounded-lg border transition-all ${
                  v.active
                    ? 'bg-emerald-500/10 border-emerald-500/50 text-slate-100 shadow-[0_0_8px_rgba(16,185,129,0.2)]'
                    : 'bg-slate-900/30 border-slate-800/60 text-slate-500'
                }`}
              >
                <div className="flex items-center justify-between mb-1">
                  <span className="font-mono text-[11px] font-bold">V{v.id}</span>
                  <span
                    className={`w-2 h-2 rounded-full ${
                      v.active
                        ? 'bg-emerald-400 shadow-[0_0_6px_#34d399]'
                        : 'bg-slate-700'
                    }`}
                  />
                </div>
                <div className="flex items-center justify-between font-mono text-[10px]">
                  <span>{v.active ? `MIDI ${v.midiKey}` : 'IDLE'}</span>
                  <span>{v.active ? `${v.frequency.toFixed(0)}Hz` : '---'}</span>
                </div>
                {/* Level mini-bar */}
                <div className="w-full bg-slate-800 h-1 rounded-full mt-1.5 overflow-hidden">
                  <div
                    className="bg-emerald-400 h-full transition-all duration-75"
                    style={{ width: `${v.envelopeLevel * 100}%` }}
                  />
                </div>
              </div>
            ))}
          </div>
        </div>

        <div className="mt-3 pt-2 border-t border-slate-800 text-[10px] font-mono text-slate-400">
          Dynamic voice allocation with oldest-voice stealing fallback.
        </div>
      </div>
    </div>
  );
};

