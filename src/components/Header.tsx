// ==============================================================================
// Header.tsx
// Navigation and top toolbar component.
//
// Features:
// - Project title and version badge.
// - Audio engine toggle button (Web Audio Context starter).
// - Architecture modal viewer button.
// ==============================================================================

import React from 'react';
import { Volume2, Power, Code2 } from 'lucide-react';

interface HeaderProps {
  isAudioStarted: boolean;
  onToggleAudio: () => void;
  onOpenArchitecture: () => void;
}

export const Header: React.FC<HeaderProps> = ({
  isAudioStarted,
  onToggleAudio,
  onOpenArchitecture,
}) => {
  return (
    <header className="bg-[#0f1318] border-b border-slate-800 px-6 py-4 flex flex-wrap items-center justify-between gap-4">
      <div className="flex items-center gap-3">
        <div className="w-10 h-10 rounded-lg bg-emerald-500/10 border border-emerald-500/30 flex items-center justify-center text-emerald-400">
          <Volume2 className="w-5 h-5" />
        </div>
        <div>
          <div className="flex items-center gap-2">
            <h1 className="font-mono text-base font-bold tracking-wide text-slate-100">
              OPENFRAMEWORKS SYNTHESIZER & VISUALIZER
            </h1>
            <span className="px-2 py-0.5 rounded text-[10px] font-mono font-semibold bg-slate-800 text-emerald-400 border border-slate-700">
              v0.11.2 Core
            </span>
          </div>
          <p className="text-xs text-slate-400">
            Real-time C++ Polyphonic DSP Sound Synthesizer & Reactive Spectrum Analyzer
          </p>
        </div>
      </div>

      <div className="flex items-center gap-3">
        {/* Button to open the UML and team architecture dialog */}
        <button
          id="view-architecture-btn"
          type="button"
          onClick={onOpenArchitecture}
          className="flex items-center gap-2 px-3 py-1.5 rounded-lg border border-slate-700 bg-slate-900/80 hover:bg-slate-800 text-slate-300 text-xs font-mono transition-colors"
        >
          <Code2 className="w-4 h-4 text-indigo-400" />
          <span>Architecture & UML</span>
        </button>

        {/* Button to start or stop the audio engine */}
        <button
          id="audio-power-btn"
          type="button"
          onClick={onToggleAudio}
          className={`flex items-center gap-2 px-4 py-1.5 rounded-lg border text-xs font-mono font-bold transition-all ${
            isAudioStarted
              ? 'bg-emerald-500 text-slate-950 border-emerald-400 shadow-[0_0_15px_rgba(52,211,153,0.4)]'
              : 'bg-slate-800 hover:bg-slate-700 text-slate-200 border-slate-600'
          }`}
        >
          <Power className={`w-4 h-4 ${isAudioStarted ? 'text-slate-950' : 'text-emerald-400'}`} />
          <span>{isAudioStarted ? 'ENGINE RUNNING (44.1kHz)' : 'START AUDIO ENGINE'}</span>
        </button>
      </div>
    </header>
  );
};

