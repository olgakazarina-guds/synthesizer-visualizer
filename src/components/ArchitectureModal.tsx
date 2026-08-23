// ==============================================================================
// ArchitectureModal.tsx
// Interactive modal dialog displaying the C++ OOP Architecture & UML Diagrams.
//
// Shows:
// - Left architecture module (Olga: Synth, Voice, Envelope).
// - Right architecture module (Mohammed: Oscillator, Waveforms, Visualizer).
// - ASCII UML class hierarchy and relationship diagram.
// - OOP concepts applied: Inheritance, Composition, Association, Polymorphism.
// ==============================================================================

import React from 'react';
import { X } from 'lucide-react';

interface ArchitectureModalProps {
  isOpen: boolean;
  onClose: () => void;
}

export const ArchitectureModal: React.FC<ArchitectureModalProps> = ({
  isOpen,
  onClose,
}) => {
  if (!isOpen) return null;

  return (
    <div className="fixed inset-0 z-50 flex items-center justify-center p-4 bg-black/70 backdrop-blur-xs">
      <div className="bg-[#0f1318] border border-slate-700 rounded-2xl max-w-3xl w-full max-h-[90vh] overflow-y-auto p-6 shadow-2xl">
        <div className="flex items-center justify-between pb-4 border-b border-slate-800">
          <div>
            <h3 className="text-lg font-bold font-mono text-slate-100">
              C++ Object-Oriented Architecture (OOP) & UML
            </h3>
            <p className="text-xs text-slate-400 mt-0.5">
              Coding Camp II (Project 2) Software Synthesizer Architecture Breakdown
            </p>
          </div>
          <button
            onClick={onClose}
            className="p-1 rounded-lg text-slate-400 hover:text-white hover:bg-slate-800 transition-colors"
          >
            <X className="w-5 h-5" />
          </button>
        </div>

        <div className="mt-5 space-y-6 text-sm text-slate-300">
          {/* Authors */}
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
            <div className="p-4 rounded-xl bg-slate-900/60 border border-slate-800">
              <h4 className="font-mono text-xs font-bold text-emerald-400 uppercase tracking-wider mb-2">
                Olga Kazarina (s-01132) — Core Audio & Voices
              </h4>
              <ul className="space-y-1.5 text-xs text-slate-400">
                <li>• <strong className="text-slate-200">Synth</strong>: Core audio engine container and stream callback.</li>
                <li>• <strong className="text-slate-200">Voice</strong>: Polyphonic note management and oscillator binding.</li>
                <li>• <strong className="text-slate-200">Envelope</strong>: ADSR volume envelope generator shaping amplitude over time.</li>
              </ul>
            </div>

            <div className="p-4 rounded-xl bg-slate-900/60 border border-slate-800">
              <h4 className="font-mono text-xs font-bold text-indigo-400 uppercase tracking-wider mb-2">
                Mohammedsaleh Ibrahim (s-01136) — Oscillators & Visualizer
              </h4>
              <ul className="space-y-1.5 text-xs text-slate-400">
                <li>• <strong className="text-slate-200">Oscillator</strong>: Abstract base class with pure virtual interface.</li>
                <li>• <strong className="text-slate-200">Waveforms</strong>: SineOscillator, SquareOscillator, SawOscillator.</li>
                <li>• <strong className="text-slate-200">Visualizer</strong>: Real-time oscilloscope waveform & FFT frequency spectrum.</li>
              </ul>
            </div>
          </div>

          {/* UML ASCII Diagram */}
          <div>
            <h4 className="font-mono text-xs font-bold text-slate-200 uppercase tracking-wider mb-2">
              UML Class Relationships
            </h4>
            <div className="bg-slate-950 p-4 rounded-xl border border-slate-800 font-mono text-[11px] text-slate-300 leading-relaxed overflow-x-auto whitespace-pre">
{`┌──────────────────────────────────────┐                   ┌──────────────────────────────────────┐
│                Synth                 │─── reads buffer ─>│              Visualizer              │
│       Audio engine, audioOut()       │                   │       Draws waveform / spectrum      │
└──────────────────────────────────────┘                   └──────────────────────────────────────┘
        │                              │
     composes                       composes
        │                              │
        ▼                              ▼
┌──────────────────────────────────────┐                   ┌──────────────────────────────────────┐
│                Voice                 │                   │        Oscillator «abstract»         │
│           One playing note           │                   │           generateSample()           │
└──────────────────────────────────────┘                   └──────────────────────────────────────┘
        │                                                                     ▲
     composes                                                                 │
        │                                                     ┌───────────────┼───────────────┐
        ▼                                                     │               │               │
┌──────────────────────────────────────┐             ┌─────────────────┐ ┌───────────────┐ ┌───────────────┐
│               Envelope               │             │ SineOscillator  │ │SquareOscillator│ │ SawOscillator │
│       Shapes volume over time        │             └─────────────────┘ └───────────────┘ └───────────────┘
└──────────────────────────────────────┘`}
            </div>
          </div>

          {/* Key OOP Principles */}
          <div className="space-y-2">
            <h4 className="font-mono text-xs font-bold text-slate-200 uppercase tracking-wider">
              OOP Design Patterns Applied
            </h4>
            <div className="grid grid-cols-1 md:grid-cols-3 gap-3 text-xs">
              <div className="p-3 bg-slate-900/40 rounded-lg border border-slate-800">
                <div className="font-mono font-bold text-emerald-400 mb-1">1. Inheritance</div>
                <div className="text-slate-400">
                  Sine, Square, and Saw classes inherit from abstract base <code className="text-slate-300">Oscillator</code> and implement <code className="text-slate-300">generateSample()</code>.
                </div>
              </div>
              <div className="p-3 bg-slate-900/40 rounded-lg border border-slate-800">
                <div className="font-mono font-bold text-indigo-400 mb-1">2. Composition</div>
                <div className="text-slate-400">
                  <code className="text-slate-300">Synth</code> owns 8 <code className="text-slate-300">Voice</code> instances and oscillator pool; each Voice owns its ADSR <code className="text-slate-300">Envelope</code>.
                </div>
              </div>
              <div className="p-3 bg-slate-900/40 rounded-lg border border-slate-800">
                <div className="font-mono font-bold text-amber-400 mb-1">3. Association</div>
                <div className="text-slate-400">
                  <code className="text-slate-300">Visualizer</code> reads audio buffers from <code className="text-slate-300">Synth</code> via <code className="text-slate-300">getBuffer()</code> without taking ownership.
                </div>
              </div>
            </div>
          </div>
        </div>

        <div className="mt-6 pt-4 border-t border-slate-800 flex justify-end">
          <button
            onClick={onClose}
            className="px-4 py-2 bg-emerald-500 hover:bg-emerald-400 text-slate-950 font-mono font-bold text-xs rounded-lg transition-colors"
          >
            Close Diagram
          </button>
        </div>
      </div>
    </div>
  );
};

