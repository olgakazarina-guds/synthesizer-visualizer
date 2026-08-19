// ==============================================================================
// PianoKeyboard.tsx
// Interactive virtual piano keyboard controller (C4 to C5 with Octave Shift).
//
// Visual & Mechanical Features:
// - Authentic hardware synth chassis with anodized casing and red velvet felt strip.
// - 3D white keys with glossy ivory finish, physical bottom bevels, and spring action.
// - 3D black keys with obsidian matte texture, raised elevation, and realistic depth.
// - Responsive multi-touch & mouse pointer capture with zero stuck notes.
// - Real-time green LED keybed under-lighting and depression physics.
// - Octave Transpose selector (Octave 3, 4, or 5).
// - Display mode toggles (Key Shortcuts, Note Names, and Frequency in Hz).
// ==============================================================================

import React, { useState } from 'react';
import { SynthEngine } from '../audio/SynthEngine';
import { ChevronLeft, ChevronRight, Eye, Music } from 'lucide-react';

interface PianoKeyboardProps {
  onNoteOn: (note: number, freq: number) => void;
  onNoteOff: (note: number) => void;
  activeNotes: Set<number>;
  octaveOffset?: number;
  onOctaveChange?: (offset: number) => void;
}

interface KeyConfig {
  baseMidi: number; // Base MIDI note in Octave 4
  baseName: string;
  isBlack: boolean;
  keyboardShortcut?: string;
  offsetPercent?: number; // Placement percentage for black keys
}

export const PianoKeyboard: React.FC<PianoKeyboardProps> = ({
  onNoteOn,
  onNoteOff,
  activeNotes,
  octaveOffset = 0,
  onOctaveChange,
}) => {
  // Label display mode: 'all' (shortcut + note + hz), 'notes' (note only), 'minimal'
  const [labelMode, setLabelMode] = useState<'all' | 'notes' | 'minimal'>('all');

  // Key configurations for one full octave C to C (8 white keys, 5 black keys)
  const baseKeys: KeyConfig[] = [
    { baseMidi: 60, baseName: 'C', isBlack: false, keyboardShortcut: 'A' },
    { baseMidi: 61, baseName: 'C#', isBlack: true, offsetPercent: 8.2, keyboardShortcut: 'W' },
    { baseMidi: 62, baseName: 'D', isBlack: false, keyboardShortcut: 'S' },
    { baseMidi: 63, baseName: 'D#', isBlack: true, offsetPercent: 21.6, keyboardShortcut: 'E' },
    { baseMidi: 64, baseName: 'E', isBlack: false, keyboardShortcut: 'D' },
    { baseMidi: 65, baseName: 'F', isBlack: false, keyboardShortcut: 'F' },
    { baseMidi: 66, baseName: 'F#', isBlack: true, offsetPercent: 50.8, keyboardShortcut: 'T' },
    { baseMidi: 67, baseName: 'G', isBlack: false, keyboardShortcut: 'G' },
    { baseMidi: 68, baseName: 'G#', isBlack: true, offsetPercent: 64.2, keyboardShortcut: 'Y' },
    { baseMidi: 69, baseName: 'A', isBlack: false, keyboardShortcut: 'H' },
    { baseMidi: 70, baseName: 'A#', isBlack: true, offsetPercent: 77.6, keyboardShortcut: 'U' },
    { baseMidi: 71, baseName: 'B', isBlack: false, keyboardShortcut: 'J' },
    { baseMidi: 72, baseName: 'C', isBlack: false, keyboardShortcut: 'K' },
  ];

  // Calculate actual MIDI note and octave number based on octaveOffset
  const currentOctaveNum = 4 + octaveOffset / 12;

  const whiteKeys = baseKeys.filter((k) => !k.isBlack);
  const blackKeys = baseKeys.filter((k) => k.isBlack);

  const handleOctaveShift = (direction: number) => {
    if (!onOctaveChange) return;
    const newOffset = Math.max(-24, Math.min(24, octaveOffset + direction * 12));
    onOctaveChange(newOffset);
  };

  return (
    <div
      id="piano-keyboard-container"
      className="bg-gradient-to-b from-[#181d24] via-[#0f1318] to-[#0a0d11] border border-slate-700/80 rounded-2xl p-4 shadow-2xl relative overflow-hidden"
    >
      {/* Top Hardware Panel Header */}
      <div className="flex flex-wrap items-center justify-between gap-3 mb-3 pb-2.5 border-b border-slate-800/80">
        <div className="flex items-center gap-2.5">
          <div className="w-6 h-6 rounded-md bg-emerald-500/10 border border-emerald-500/30 flex items-center justify-center text-emerald-400">
            <Music className="w-3.5 h-3.5" />
          </div>
          <div>
            <div className="flex items-center gap-2">
              <span className="font-mono text-xs font-bold tracking-wider text-slate-200">
                HARDWARE KEYBED CONTROLLER
              </span>
              <span className="px-1.5 py-0.5 rounded text-[9px] font-mono font-bold bg-emerald-950/80 text-emerald-300 border border-emerald-800/50">
                VELOCITY SENSITIVE
              </span>
            </div>
            <p className="text-[11px] font-mono text-slate-400">
              Home-row keys [A W S E D F T G Y H U J K] ➔ Octave {currentOctaveNum}
            </p>
          </div>
        </div>

        {/* Controller Action Tools (Octave Shift & Label Toggle) */}
        <div className="flex items-center gap-2">
          {/* Octave Transpose Controls */}
          <div className="flex items-center bg-slate-900 border border-slate-700/80 rounded-lg p-0.5 font-mono text-xs shadow-inner">
            <button
              id="octave-down-btn"
              type="button"
              onClick={() => handleOctaveShift(-1)}
              disabled={octaveOffset <= -24}
              title="Lower Octave (-12 semitones)"
              className="px-2 py-1 rounded hover:bg-slate-800 text-slate-300 disabled:opacity-30 disabled:hover:bg-transparent transition-colors flex items-center gap-0.5 text-[11px]"
            >
              <ChevronLeft className="w-3.5 h-3.5" />
              <span>OCT -</span>
            </button>
            <span className="px-2 py-0.5 text-[11px] font-bold text-emerald-400 min-w-[56px] text-center border-x border-slate-800">
              OCT {currentOctaveNum}
            </span>
            <button
              id="octave-up-btn"
              type="button"
              onClick={() => handleOctaveShift(1)}
              disabled={octaveOffset >= 24}
              title="Raise Octave (+12 semitones)"
              className="px-2 py-1 rounded hover:bg-slate-800 text-slate-300 disabled:opacity-30 disabled:hover:bg-transparent transition-colors flex items-center gap-0.5 text-[11px]"
            >
              <span>OCT +</span>
              <ChevronRight className="w-3.5 h-3.5" />
            </button>
          </div>

          {/* Label Display Mode Toggle */}
          <button
            id="key-label-mode-toggle"
            type="button"
            onClick={() =>
              setLabelMode((prev) =>
                prev === 'all' ? 'notes' : prev === 'notes' ? 'minimal' : 'all'
              )
            }
            className="flex items-center gap-1.5 px-2.5 py-1.5 bg-slate-900 hover:bg-slate-800 border border-slate-700/80 rounded-lg text-slate-300 font-mono text-[11px] transition-colors"
          >
            <Eye className="w-3 h-3 text-emerald-400" />
            <span className="uppercase">{labelMode} Labels</span>
          </button>
        </div>
      </div>

      {/* Piano Chassis Frame */}
      <div className="relative rounded-xl bg-slate-950 p-2.5 border border-slate-800/90 shadow-inner">
        {/* Red Velvet / Felt Dampener Bar (Authentic acoustic/synth detail) */}
        <div className="h-2 w-full bg-gradient-to-r from-red-950 via-red-800 to-red-950 rounded-t-sm border-b border-red-900/60 shadow-[inset_0_1px_2px_rgba(0,0,0,0.6)] flex items-center justify-center">
          <div className="h-[1px] w-[96%] bg-red-500/30"></div>
        </div>

        {/* Piano Keys Interactive Stage */}
        <div id="piano-keys-stage" className="relative h-48 w-full select-none">
          {/* White Keys Row */}
          <div className="flex h-full w-full gap-[3px] bg-slate-900/40 p-0.5 rounded-b-lg">
            {whiteKeys.map((k, idx) => {
              const actualMidi = k.baseMidi + octaveOffset;
              const noteOctave = idx === whiteKeys.length - 1 ? currentOctaveNum + 1 : currentOctaveNum;
              const noteName = `${k.baseName}${noteOctave}`;
              const noteFreq = SynthEngine.midiToFreq(actualMidi);
              const isActive = activeNotes.has(actualMidi);

              return (
                <button
                  key={actualMidi}
                  id={`piano-key-${actualMidi}`}
                  type="button"
                  onPointerDown={(e) => {
                    e.currentTarget.setPointerCapture(e.pointerId);
                    onNoteOn(actualMidi, noteFreq);
                  }}
                  onPointerUp={() => onNoteOff(actualMidi)}
                  onPointerLeave={() => onNoteOff(actualMidi)}
                  className={`flex-1 h-full rounded-b-md flex flex-col justify-between items-center py-2.5 px-1 transition-all duration-75 relative group cursor-pointer ${
                    isActive
                      ? 'bg-gradient-to-b from-emerald-100 via-emerald-300 to-emerald-400 text-slate-950 shadow-[0_0_22px_rgba(52,211,153,0.7),inset_0_-2px_4px_rgba(0,0,0,0.2)] border-x border-t border-emerald-400 border-b-2 border-b-emerald-600 translate-y-1.5'
                      : 'bg-gradient-to-b from-slate-100 via-[#f8fafc] to-[#e2e8f0] hover:from-white hover:to-slate-200 text-slate-800 shadow-[0_4px_6px_rgba(0,0,0,0.3),inset_0_1px_1px_rgba(255,255,255,0.9)] border-x border-t border-slate-300 border-b-[6px] border-b-slate-400/80 active:translate-y-1 active:border-b-2'
                  }`}
                >
                  {/* Top reflection line */}
                  <div
                    className={`w-full h-[2px] rounded-full ${
                      isActive ? 'bg-emerald-200' : 'bg-white/90'
                    }`}
                  />

                  {/* Center Keyboard Shortcut Keycap Pill */}
                  {labelMode === 'all' && (
                    <div
                      className={`font-mono text-[10px] font-bold px-1.5 py-0.5 rounded shadow-xs transition-colors ${
                        isActive
                          ? 'bg-emerald-950 text-emerald-200 border border-emerald-600'
                          : 'bg-slate-200/90 text-slate-700 border border-slate-300 group-hover:bg-slate-300'
                      }`}
                    >
                      {k.keyboardShortcut}
                    </div>
                  )}

                  {/* Bottom Note Name & Pitch */}
                  <div className="flex flex-col items-center gap-0.5 mb-1">
                    {labelMode !== 'minimal' && (
                      <span
                        className={`font-mono text-xs font-black tracking-tight ${
                          isActive ? 'text-slate-950 scale-105' : 'text-slate-800'
                        }`}
                      >
                        {noteName}
                      </span>
                    )}
                    {labelMode === 'all' && (
                      <span
                        className={`font-mono text-[9px] font-semibold ${
                          isActive ? 'text-emerald-950' : 'text-slate-500'
                        }`}
                      >
                        {noteFreq >= 1000 ? `${(noteFreq / 1000).toFixed(2)}k` : `${noteFreq.toFixed(0)}Hz`}
                      </span>
                    )}
                  </div>
                </button>
              );
            })}
          </div>

          {/* Black Keys Floating Row */}
          <div className="absolute top-0 left-0 right-0 h-[62%] pointer-events-none px-1">
            {blackKeys.map((k) => {
              const actualMidi = k.baseMidi + octaveOffset;
              const noteName = `${k.baseName}${currentOctaveNum}`;
              const noteFreq = SynthEngine.midiToFreq(actualMidi);
              const isActive = activeNotes.has(actualMidi);

              return (
                <button
                  key={actualMidi}
                  id={`piano-key-black-${actualMidi}`}
                  type="button"
                  style={{ left: `${k.offsetPercent}%` }}
                  onPointerDown={(e) => {
                    e.currentTarget.setPointerCapture(e.pointerId);
                    onNoteOn(actualMidi, noteFreq);
                  }}
                  onPointerUp={() => onNoteOff(actualMidi)}
                  onPointerLeave={() => onNoteOff(actualMidi)}
                  className={`absolute top-0 w-[7.2%] h-full rounded-b-md flex flex-col justify-between items-center py-2 px-0.5 transition-all duration-75 pointer-events-auto z-20 cursor-pointer group ${
                    isActive
                      ? 'bg-gradient-to-b from-emerald-600 via-emerald-500 to-emerald-400 text-white shadow-[0_0_24px_rgba(16,185,129,0.9),inset_0_-2px_4px_rgba(0,0,0,0.4)] border-x border-t border-emerald-400 border-b-2 border-b-emerald-700 translate-y-1'
                      : 'bg-gradient-to-b from-[#222834] via-[#151921] to-[#0b0e14] hover:from-[#2c3444] text-slate-300 shadow-[0_6px_10px_rgba(0,0,0,0.6),inset_0_1px_1px_rgba(255,255,255,0.15)] border-x border-t border-slate-700/80 border-b-[6px] border-b-black active:translate-y-1 active:border-b-2'
                  }`}
                >
                  {/* Top gloss highlight */}
                  <div
                    className={`w-[85%] h-[2px] rounded-full ${
                      isActive ? 'bg-emerald-300' : 'bg-slate-500/40'
                    }`}
                  />

                  {/* Black Key Shortcut Badge */}
                  {labelMode === 'all' && (
                    <div
                      className={`font-mono text-[9px] font-bold px-1 py-0.5 rounded shadow-xs ${
                        isActive
                          ? 'bg-emerald-950 text-emerald-200 border border-emerald-600'
                          : 'bg-slate-800/90 text-slate-400 border border-slate-700 group-hover:text-slate-200'
                      }`}
                    >
                      {k.keyboardShortcut}
                    </div>
                  )}

                  {/* Black Key Name Label */}
                  <div className="flex flex-col items-center mb-1">
                    {labelMode !== 'minimal' && (
                      <span
                        className={`font-mono text-[10px] font-bold tracking-tight ${
                          isActive ? 'text-white' : 'text-slate-200'
                        }`}
                      >
                        {noteName}
                      </span>
                    )}
                    {labelMode === 'all' && (
                      <span
                        className={`font-mono text-[8px] font-medium ${
                          isActive ? 'text-emerald-100' : 'text-slate-500'
                        }`}
                      >
                        {noteFreq.toFixed(0)}
                      </span>
                    )}
                  </div>
                </button>
              );
            })}
          </div>
        </div>
      </div>
    </div>
  );
};


