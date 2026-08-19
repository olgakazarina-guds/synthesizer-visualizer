// ==============================================================================
// App.tsx
// Root application component for the Software Synthesizer & Visualizer.
//
// Responsibilities:
// - Manages global audio engine lifecycle (SynthEngine and VisualizerEngine).
// - Handles computer keyboard events (A-K for notes, 1-3 for waveforms).
// - Manages user parameter state (wave type, ADSR envelope, master volume).
// - Organizes layout: Header, Visualizer Canvas, Pitch Ribbon, Piano, and Synth Controls.
// ==============================================================================

import React, { useState, useEffect, useCallback } from 'react';
import { WaveType, ADSRParams, VoiceState } from './types';
import { SynthEngine } from './audio/SynthEngine';
import { VisualizerEngine } from './audio/VisualizerEngine';
import { Header } from './components/Header';
import { VisualizerCanvas } from './components/VisualizerCanvas';
import { SynthControls } from './components/SynthControls';
import { PianoKeyboard } from './components/PianoKeyboard';
import { PitchRibbon } from './components/PitchRibbon';
import { ArchitectureModal } from './components/ArchitectureModal';

export const App: React.FC = () => {
  // Instantiates audio engine and FFT visualizer
  const [synth] = useState<SynthEngine>(() => new SynthEngine(44100, 512, 8));
  const [visualizer] = useState<VisualizerEngine>(() => new VisualizerEngine(256));

  // Application state
  const [isAudioStarted, setIsAudioStarted] = useState<boolean>(false);
  const [currentWaveType, setCurrentWaveType] = useState<WaveType>(WaveType.SINE);
  const [masterVolume, setMasterVolume] = useState<number>(0.8);
  const [adsr, setAdsr] = useState<ADSRParams>({
    attack: 0.05,
    decay: 0.1,
    sustain: 0.7,
    release: 0.3,
  });

  const [activeNotes, setActiveNotes] = useState<Set<number>>(new Set());
  const [activeMouseNote, setActiveMouseNote] = useState<number | null>(null);
  const [voiceStates, setVoiceStates] = useState<VoiceState[]>([]);
  const [octaveOffset, setOctaveOffset] = useState<number>(0);
  const [isArchModalOpen, setIsArchModalOpen] = useState<boolean>(false);

  // Home row keyboard mappings matching ofApp::buildKeyMap()
  const keyToNoteMap: Record<string, number> = {
    a: 60, // C4
    w: 61, // C#4
    s: 62, // D4
    e: 63, // D#4
    d: 64, // E4
    f: 65, // F4
    t: 66, // F#4
    g: 67, // G4
    y: 68, // G#4
    h: 69, // A4
    u: 70, // A#4
    j: 71, // B4
    k: 72, // C5
  };

  // Start audio on user gesture
  const handleStartAudio = async () => {
    const success = await synth.initAudio();
    if (success) {
      setIsAudioStarted(true);
    }
  };

  // Trigger musical note
  const handleNoteOn = useCallback(
    (midiNote: number, frequency?: number) => {
      if (!synth.isStarted()) {
        synth.initAudio().then(() => setIsAudioStarted(true));
      }
      synth.noteOn(midiNote, frequency);
      setActiveNotes((prev) => new Set(prev).add(midiNote));
    },
    [synth]
  );

  // Release musical note
  const handleNoteOff = useCallback(
    (midiNote: number) => {
      synth.noteOff(midiNote);
      setActiveNotes((prev) => {
        const next = new Set(prev);
        next.delete(midiNote);
        return next;
      });
    },
    [synth]
  );

  // Change waveform type (Sine, Square, Sawtooth)
  const handleWaveTypeChange = (type: WaveType) => {
    synth.setWaveType(type);
    setCurrentWaveType(type);
  };

  // Update ADSR parameters
  const handleAdsrChange = (newAdsr: ADSRParams) => {
    synth.setADSR(newAdsr.attack, newAdsr.decay, newAdsr.sustain, newAdsr.release);
    setAdsr(newAdsr);
  };

  // Update master volume
  const handleVolumeChange = (vol: number) => {
    synth.setMasterVolume(vol);
    setMasterVolume(vol);
  };

  // Keyboard shortcut listeners matching ofApp::keyPressed and ofApp::keyReleased
  useEffect(() => {
    const pressedKeys = new Map<string, number>();

    const onKeyDown = (e: KeyboardEvent) => {
      if (e.repeat) return;
      const key = e.key.toLowerCase();

      // Waveform shortcuts (1, 2, 3)
      if (key === '1') {
        handleWaveTypeChange(WaveType.SINE);
        return;
      }
      if (key === '2') {
        handleWaveTypeChange(WaveType.SQUARE);
        return;
      }
      if (key === '3') {
        handleWaveTypeChange(WaveType.SAW);
        return;
      }

      // Musical note keys (A through K) with current octave transpose
      if (keyToNoteMap[key] !== undefined && !pressedKeys.has(key)) {
        const midi = keyToNoteMap[key] + octaveOffset;
        pressedKeys.set(key, midi);
        handleNoteOn(midi);
      }
    };

    const onKeyUp = (e: KeyboardEvent) => {
      const key = e.key.toLowerCase();
      if (pressedKeys.has(key)) {
        const midi = pressedKeys.get(key)!;
        pressedKeys.delete(key);
        handleNoteOff(midi);
      }
    };

    window.addEventListener('keydown', onKeyDown);
    window.addEventListener('keyup', onKeyUp);

    return () => {
      window.removeEventListener('keydown', onKeyDown);
      window.removeEventListener('keyup', onKeyUp);
    };
  }, [handleNoteOn, handleNoteOff, octaveOffset]);

  // Periodic polling of voice state for the UI indicator matrix
  useEffect(() => {
    const interval = setInterval(() => {
      setVoiceStates(synth.getVoiceStates());
    }, 50);
    return () => clearInterval(interval);
  }, [synth]);

  return (
    <div className="min-h-screen bg-[#14181e] text-slate-100 flex flex-col font-sans">
      <Header
        isAudioStarted={isAudioStarted}
        onToggleAudio={handleStartAudio}
        onOpenArchitecture={() => setIsArchModalOpen(true)}
      />

      <main className="flex-1 max-w-7xl w-full mx-auto p-4 md:p-6 space-y-5">
        {/* Top: Dual Oscilloscope & Spectrum Visualizer */}
        <div className="h-[340px]">
          <VisualizerCanvas
            visualizer={visualizer}
            buffer={synth.getBuffer()}
            isAudioStarted={isAudioStarted}
          />
        </div>

        {/* Middle: Continuous Pitch Ribbon Controller */}
        <PitchRibbon
          onNoteOn={(note, freq) => {
            setActiveMouseNote(note);
            handleNoteOn(note, freq);
          }}
          onNoteOff={(note) => {
            setActiveMouseNote(null);
            handleNoteOff(note);
          }}
          activeMouseNote={activeMouseNote}
        />

        {/* Piano Keyboard Controller with 3D Keys & Octave Controls */}
        <PianoKeyboard
          onNoteOn={handleNoteOn}
          onNoteOff={handleNoteOff}
          activeNotes={activeNotes}
          octaveOffset={octaveOffset}
          onOctaveChange={setOctaveOffset}
        />

        {/* Bottom: Waveform, ADSR, & Voice Allocation Controls */}
        <SynthControls
          currentWaveType={currentWaveType}
          onWaveTypeChange={handleWaveTypeChange}
          adsr={adsr}
          onAdsrChange={handleAdsrChange}
          masterVolume={masterVolume}
          onVolumeChange={handleVolumeChange}
          voiceStates={voiceStates}
        />
      </main>

      {/* Architecture UML Reference Modal */}
      <ArchitectureModal
        isOpen={isArchModalOpen}
        onClose={() => setIsArchModalOpen(false)}
      />
    </div>
  );
};

