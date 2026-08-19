import React from 'react';
import { SynthEngine } from '../audio/SynthEngine';

interface PianoKeyboardProps {
  onNoteOn: (note: number, freq: number) => void;
  onNoteOff: (note: number) => void;
  activeNotes: Set<number>;
}

interface KeyConfig {
  midi: number;
  name: string;
  isBlack: boolean;
  keyboardShortcut?: string;
  offsetPercent?: number; // for black keys position
}

export const PianoKeyboard: React.FC<PianoKeyboardProps> = ({
  onNoteOn,
  onNoteOff,
  activeNotes,
}) => {
  // 2-octave piano range from C4 (MIDI 60) to C5 (MIDI 72), plus home-row keys a s d f g h j k mapped
  const keys: KeyConfig[] = [
    { midi: 60, name: 'C4', isBlack: false, keyboardShortcut: 'A' },
    { midi: 61, name: 'C#4', isBlack: true, offsetPercent: 8.5, keyboardShortcut: 'W' },
    { midi: 62, name: 'D4', isBlack: false, keyboardShortcut: 'S' },
    { midi: 63, name: 'D#4', isBlack: true, offsetPercent: 22.5, keyboardShortcut: 'E' },
    { midi: 64, name: 'E4', isBlack: false, keyboardShortcut: 'D' },
    { midi: 65, name: 'F4', isBlack: false, keyboardShortcut: 'F' },
    { midi: 66, name: 'F#4', isBlack: true, offsetPercent: 51.5, keyboardShortcut: 'T' },
    { midi: 67, name: 'G4', isBlack: false, keyboardShortcut: 'G' },
    { midi: 68, name: 'G#4', isBlack: true, offsetPercent: 65.5, keyboardShortcut: 'Y' },
    { midi: 69, name: 'A4', isBlack: false, keyboardShortcut: 'H' },
    { midi: 70, name: 'A#4', isBlack: true, offsetPercent: 79.5, keyboardShortcut: 'U' },
    { midi: 71, name: 'B4', isBlack: false, keyboardShortcut: 'J' },
    { midi: 72, name: 'C5', isBlack: false, keyboardShortcut: 'K' },
  ];

  const whiteKeys = keys.filter((k) => !k.isBlack);
  const blackKeys = keys.filter((k) => k.isBlack);

  return (
    <div id="piano-keyboard-container" className="bg-[#0f1318] border border-slate-800 rounded-xl p-4 shadow-lg">
      <div className="flex items-center justify-between mb-3">
        <div className="flex items-center gap-2">
          <span className="font-mono text-xs font-semibold tracking-wider text-slate-300">
            KEYBOARD CONTROLLER (KEYS A S D F G H J K ➔ C4..C5)
          </span>
        </div>
        <div className="text-xs font-mono text-slate-400">
          Click keys or use computer keyboard
        </div>
      </div>

      <div id="piano-keys-wrapper" className="relative h-44 w-full select-none">
        {/* White Keys */}
        <div className="flex h-full w-full gap-1">
          {whiteKeys.map((k) => {
            const isActive = activeNotes.has(k.midi);
            return (
              <button
                key={k.midi}
                id={`piano-key-${k.midi}`}
                type="button"
                onPointerDown={(e) => {
                  e.currentTarget.setPointerCapture(e.pointerId);
                  onNoteOn(k.midi, SynthEngine.midiToFreq(k.midi));
                }}
                onPointerUp={() => onNoteOff(k.midi)}
                onPointerLeave={() => onNoteOff(k.midi)}
                className={`flex-1 h-full rounded-b-md flex flex-col justify-between items-center py-2 transition-all duration-75 border ${
                  isActive
                    ? 'bg-emerald-400 text-slate-950 border-emerald-300 shadow-[0_0_15px_rgba(52,211,153,0.5)] translate-y-1'
                    : 'bg-slate-100 hover:bg-slate-200 text-slate-800 border-slate-300 active:bg-emerald-200'
                }`}
              >
                <span className="font-mono text-[11px] font-bold opacity-60">
                  {k.keyboardShortcut}
                </span>
                <span className="font-mono text-xs font-bold">{k.name}</span>
              </button>
            );
          })}
        </div>

        {/* Black Keys */}
        <div className="absolute top-0 left-0 right-0 h-[60%] pointer-events-none">
          {blackKeys.map((k) => {
            const isActive = activeNotes.has(k.midi);
            return (
              <button
                key={k.midi}
                id={`piano-key-black-${k.midi}`}
                type="button"
                style={{ left: `${k.offsetPercent}%` }}
                onPointerDown={(e) => {
                  e.currentTarget.setPointerCapture(e.pointerId);
                  onNoteOn(k.midi, SynthEngine.midiToFreq(k.midi));
                }}
                onPointerUp={() => onNoteOff(k.midi)}
                onPointerLeave={() => onNoteOff(k.midi)}
                className={`absolute top-0 w-[7%] h-full rounded-b-md flex flex-col justify-between items-center py-1.5 transition-all duration-75 border pointer-events-auto z-20 ${
                  isActive
                    ? 'bg-emerald-500 text-white border-emerald-400 shadow-[0_0_15px_rgba(16,185,129,0.8)] translate-y-0.5'
                    : 'bg-slate-900 hover:bg-slate-800 text-slate-300 border-slate-700 active:bg-emerald-600'
                }`}
              >
                <span className="font-mono text-[9px] font-bold text-slate-400">
                  {k.keyboardShortcut}
                </span>
                <span className="font-mono text-[10px] font-bold text-slate-200">{k.name}</span>
              </button>
            );
          })}
        </div>
      </div>
    </div>
  );
};
