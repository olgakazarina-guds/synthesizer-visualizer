// ==============================================================================
// PitchRibbon.tsx
// Continuous horizontal pitch-bend touch/mouse controller (MIDI 48 to 72).
//
// Matches C++ ofApp mouseDragged logic:
// - Converts horizontal pixel position to MIDI note number (C3 to C5).
// - Allows continuous expressive glissando and pitch sliding.
// ==============================================================================

import React, { useRef } from 'react';
import { SynthEngine } from '../audio/SynthEngine';
import { MousePointer } from 'lucide-react';

interface PitchRibbonProps {
  onNoteOn: (note: number, freq: number) => void;
  onNoteOff: (note: number) => void;
  activeMouseNote: number | null;
}

export const PitchRibbon: React.FC<PitchRibbonProps> = ({
  onNoteOn,
  onNoteOff,
  activeMouseNote,
}) => {
  const ribbonRef = useRef<HTMLDivElement | null>(null);
  const isDraggingRef = useRef<boolean>(false);
  const currentNoteRef = useRef<number>(-1);

  // Exact formula from ofApp::xToMidiNote:
  // ofMap(x, 0, width, 48, 72, true) -> MIDI 48 (C3) to MIDI 72 (C5)
  const calculateMidiNote = (clientX: number): { note: number; freq: number; pct: number } => {
    if (!ribbonRef.current) return { note: 60, freq: 261.63, pct: 0.5 };
    const rect = ribbonRef.current.getBoundingClientRect();
    const x = Math.max(0, Math.min(rect.width, clientX - rect.left));
    const pct = x / rect.width;
    const note = Math.round(48 + pct * (72 - 48));
    const freq = SynthEngine.midiToFreq(note);
    return { note, freq, pct };
  };

  const handlePointerDown = (e: React.PointerEvent<HTMLDivElement>) => {
    e.currentTarget.setPointerCapture(e.pointerId);
    isDraggingRef.current = true;
    const { note, freq } = calculateMidiNote(e.clientX);
    currentNoteRef.current = note;
    onNoteOn(note, freq);
  };

  const handlePointerMove = (e: React.PointerEvent<HTMLDivElement>) => {
    if (!isDraggingRef.current) return;
    const { note, freq } = calculateMidiNote(e.clientX);
    if (note !== currentNoteRef.current) {
      if (currentNoteRef.current !== -1) {
        onNoteOff(currentNoteRef.current);
      }
      currentNoteRef.current = note;
      onNoteOn(note, freq);
    }
  };

  const handlePointerUp = () => {
    if (isDraggingRef.current) {
      isDraggingRef.current = false;
      if (currentNoteRef.current !== -1) {
        onNoteOff(currentNoteRef.current);
        currentNoteRef.current = -1;
      }
    }
  };

  // Note markers along the ribbon from MIDI 48 to 72
  const markers = [
    { midi: 48, name: 'C3' },
    { midi: 52, name: 'E3' },
    { midi: 55, name: 'G3' },
    { midi: 60, name: 'C4' },
    { midi: 64, name: 'E4' },
    { midi: 67, name: 'G4' },
    { midi: 72, name: 'C5' },
  ];

  return (
    <div id="pitch-ribbon-container" className="bg-[#0f1318] border border-slate-800 rounded-xl p-4 shadow-lg">
      <div className="flex items-center justify-between mb-2">
        <div className="flex items-center gap-2">
          <MousePointer className="w-4 h-4 text-emerald-400" />
          <span className="font-mono text-xs font-semibold tracking-wider text-slate-300">
            CONTINUOUS PITCH RIBBON (MIDI 48..72 / C3..C5)
          </span>
        </div>
        <div className="font-mono text-xs text-slate-400">
          {activeMouseNote !== null ? (
            <span className="text-emerald-400 font-bold">
              Playing Note: MIDI {activeMouseNote} ({SynthEngine.midiToFreq(activeMouseNote).toFixed(1)} Hz)
            </span>
          ) : (
            'Click & drag horizontally to pitch-bend'
          )}
        </div>
      </div>

      <div
        id="pitch-ribbon-strip"
        ref={ribbonRef}
        onPointerDown={handlePointerDown}
        onPointerMove={handlePointerMove}
        onPointerUp={handlePointerUp}
        onPointerCancel={handlePointerUp}
        className="relative h-16 w-full bg-gradient-to-r from-slate-900 via-slate-800 to-slate-900 rounded-lg border border-slate-700 cursor-ew-resize overflow-hidden touch-none select-none hover:border-emerald-500/50 transition-colors"
      >
        {/* Subtle grid lines */}
        <div className="absolute inset-0 flex justify-between pointer-events-none px-4">
          {markers.map((m) => {
            const pct = ((m.midi - 48) / (72 - 48)) * 100;
            return (
              <div
                key={m.midi}
                className="absolute top-0 bottom-0 flex flex-col items-center justify-between py-1"
                style={{ left: `${pct}%`, transform: 'translateX(-50%)' }}
              >
                <div className="w-[1px] h-3 bg-slate-600"></div>
                <span className="font-mono text-[10px] font-medium text-slate-400 select-none">
                  {m.name}
                </span>
                <div className="w-[1px] h-3 bg-slate-600"></div>
              </div>
            );
          })}
        </div>

        {/* Active cursor indicator when dragging */}
        {activeMouseNote !== null && (
          <div
            className="absolute top-0 bottom-0 w-3 bg-emerald-400/80 shadow-[0_0_12px_rgba(52,211,153,0.8)] pointer-events-none rounded-full transition-all duration-75"
            style={{
              left: `${((activeMouseNote - 48) / (72 - 48)) * 100}%`,
              transform: 'translateX(-50%)',
            }}
          />
        )}
      </div>
    </div>
  );
};

