# Polyphonic Software Synthesizer & Reactive Visualizer

A real-time C++ polyphonic audio synthesizer and real-time audio visualizer developed with **openFrameworks v0.11.2** for **Coding Camp II (Project 2)**.

Includes an interactive companion Web Audio application for cross-platform simulation, visualization, and architectural inspection.

---

## 📑 Table of Contents
1. [Project Overview](#-project-overview)
2. [Key Features](#-key-features)
3. [Authors & Task Division](#-authors--task-division)
4. [Object-Oriented Architecture (OOP)](#-object-oriented-architecture-oop)
5. [Mathematical & DSP Foundations](#-mathematical--dsp-foundations)
6. [Interactive Controls](#-interactive-controls)
7. [Repository File Structure](#-repository-file-structure)
8. [Build & Installation Guide](#-build--installation-guide)
9. [Team Interface Contracts](#-team-interface-contracts)

---

## 🎯 Project Overview

This project implements a complete digital audio synthesis workstation from scratch in modern C++ within openFrameworks. It provides real-time sound generation through band-limited oscillators, a polyphonic voice allocator with dynamic voice stealing, customizable 4-stage ADSR amplitude envelopes, and an integrated real-time visualizer providing simultaneous time-domain (oscilloscope) and frequency-domain (Discrete Fourier Transform FFT) rendering.

---

## ✨ Key Features

- **8-Voice Polyphony**: Play multiple notes simultaneously with automatic least-recently-used voice allocation and voice stealing.
- **Waveform Synthesis Engine**:
  - **Sine Wave**: Pure fundamental frequency utilizing trigonometric generation.
  - **Square Wave**: Odd-harmonic rich pulse synthesis.
  - **Sawtooth Wave**: Full-harmonic ramp generator.
- **ADSR Envelope Generator**: 4-stage finite-state machine (Attack, Decay, Sustain, Release) providing realistic instrument dynamics.
- **Real-Time Visualizer**:
  - **Time-Domain Oscilloscope**: Continuous sample wave buffer rendering with zero-crossing alignment.
  - **Frequency-Domain Spectrum Analyzer**: 128-bin DFT / FFT magnitude spectrum with harmonic color grading.
- **Dual Performance Controllers**:
  - **3D Hardware Keybed Controller**: Chromatic keyboard (C4 to C5) with octave transpose and computer keyboard shortcuts.
  - **Continuous Pitch Ribbon**: Continuous glissando frequency controller (MIDI 48 to 72 / 130 Hz to 523 Hz).

---

## 👥 Authors & Task Division

### **Olga — Audio Engine, Polyphonic Voice Management & ADSR Envelope**
- **`Synth` (`Synth.h`, `Synth.cpp`)**: Master synthesizer container managing the polyphonic voice array, waveform modes, master volume, and the core hardware audio callback `audioOut(ofSoundBuffer&)`.
- **`Voice` (`Voice.h`, `Voice.cpp`)**: Individual voice lifecycle controller, note trigger/release handlers, and oscillator-envelope sample multiplication.
- **`Envelope` (`Envelope.h`, `Envelope.cpp`)**: 4-stage ADSR finite-state machine with exponential curve calculations.
- **Integration**: Architecture contract coordination and parameter synchronization.

### **Mohammed — Oscillator DSP & Real-Time Visualizer**
- **`Oscillator` (`Oscillator.h`)**: Abstract base class defining the pure virtual `generateSample()` interface, phase accumulator, and sample rate.
- **Waveform Subclasses (`SineOscillator.h`, `SquareOscillator.h`, `SawOscillator.h`)**: Concrete implementations of Sine, Square, and Sawtooth mathematical algorithms using polymorphism.
- **`Visualizer` (`Visualizer.h`, `Visualizer.cpp`)**: Real-time oscilloscope waveform and 128-bin DFT frequency spectrum rendering.

### **Joint Collaboration**
- Interface synchronization between Audio Engine and Visualizer buffers.
- Unified cross-platform build configuration (`.gitignore`, openFrameworks Project Generator setup).
- Lab report, user manual, and performance profiling.

---

## 🏗️ Object-Oriented Architecture (OOP)

The application strictly adheres to clean Object-Oriented Programming (OOP) design patterns:

```text
┌─────────────────────────────────────────┐                   ┌─────────────────────────────────────────┐
│                 Synth                   │─── reads buffer ─>│               Visualizer                │
│        Master Audio Engine Callback     │                   │       Oscilloscope & FFT Spectrum       │
└─────────────────────────────────────────┘                   └─────────────────────────────────────────┘
         │                               │
      composes                        composes
      [8 Voices]                     [3 Oscillators]
         │                               │
         ▼                               ▼
┌─────────────────────────────────────────┐                   ┌─────────────────────────────────────────┐
│                 Voice                   │                   │          Oscillator «abstract»          │
│            One Musical Note             │                   │      virtual generateSample() = 0       │
└─────────────────────────────────────────┘                   └─────────────────────────────────────────┘
         │                                                                         ▲
      composes                                                                     │
         │                                                        ┌────────────────┼────────────────┐
         ▼                                                        │                │                │
┌─────────────────────────────────────────┐             ┌──────────────────┐ ┌───────────┐ ┌────────────────┐
│               Envelope                  │             │  SineOscillator  │ │SquareOsc  │ │ SawOscillator  │
│        ADSR State Machine               │             └──────────────────┘ └───────────┘ └────────────────┘
└─────────────────────────────────────────┘
```

### OOP Principles Applied:
1. **Inheritance & Polymorphism (`is-a`)**:
   - `Oscillator` acts as an abstract base class with a pure virtual method `virtual float generateSample() = 0`.
   - `SineOscillator`, `SquareOscillator`, and `SawOscillator` inherit from `Oscillator` and override `generateSample()`.
2. **Composition (`has-a`)**:
   - `Synth` composes eight `Voice` objects and three `Oscillator` instances.
   - Each `Voice` composes an `Envelope` to shape amplitude over time.
3. **Association & Loose Coupling (`uses`)**:
   - `Visualizer` receives a read-only buffer reference from `Synth` without tight coupling to voice internals.

---

## 📐 Mathematical & DSP Foundations

### 1. Phase Accumulation & Frequency Mapping
For an oscillator operating at frequency $f$ and sample rate $f_s$:
$$\Delta\text{phase} = \frac{2\pi \cdot f}{f_s}$$
$$\text{phase} \leftarrow (\text{phase} + \Delta\text{phase}) \pmod{2\pi}$$

MIDI note number $m$ to frequency $f$ (in Hertz):
$$f(m) = 440 \cdot 2^{\frac{m - 69}{12}}$$

### 2. Waveform Formulas
- **Sine Wave**:
  $$y(t) = \sin(\text{phase})$$
- **Square Wave**:
  $$y(t) = \begin{cases} 1.0 & \text{if } \text{phase} < \pi \\ -1.0 & \text{if } \text{phase} \ge \pi \end{cases}$$
- **Sawtooth Wave**:
  $$y(t) = 1.0 - \left( \frac{\text{phase}}{\pi} \right)$$

### 3. ADSR Envelope Calculations
- **Attack**: Linear rise from $0.0 \to 1.0$ over $T_{\text{attack}}$ seconds.
- **Decay**: Exponential drop from $1.0 \to \text{sustain}$ level over $T_{\text{decay}}$ seconds.
- **Sustain**: Constant amplitude held while key remains pressed.
- **Release**: Exponential fade from current level $\to 0.0$ over $T_{\text{release}}$ seconds.

### 4. Discrete Fourier Transform (DFT / FFT)
$$X[k] = \sum_{n=0}^{N-1} x[n] \cdot e^{-i \frac{2\pi}{N} k n} = \sum_{n=0}^{N-1} x[n] \left( \cos\left(\frac{2\pi kn}{N}\right) - i \sin\left(\frac{2\pi kn}{N}\right) \right)$$
$$\text{Magnitude}[k] = \frac{\sqrt{\text{Re}[k]^2 + \text{Im}[k]^2}}{N}$$

---

## 🎮 Interactive Controls

### Computer Keyboard Shortcuts
| Key | Action | Note / Pitch |
| :--- | :--- | :--- |
| **`A`** | White Key | C4 (MIDI 60 / 261.63 Hz) |
| **`W`** | Black Key | C#4 (MIDI 61 / 277.18 Hz) |
| **`S`** | White Key | D4 (MIDI 62 / 293.66 Hz) |
| **`E`** | Black Key | D#4 (MIDI 63 / 311.13 Hz) |
| **`D`** | White Key | E4 (MIDI 64 / 329.63 Hz) |
| **`F`** | White Key | F4 (MIDI 65 / 349.23 Hz) |
| **`T`** | Black Key | F#4 (MIDI 66 / 369.99 Hz) |
| **`G`** | White Key | G4 (MIDI 67 / 392.00 Hz) |
| **`Y`** | Black Key | G#4 (MIDI 68 / 415.30 Hz) |
| **`H`** | White Key | A4 (MIDI 69 / 440.00 Hz) |
| **`U`** | Black Key | A#4 (MIDI 70 / 466.16 Hz) |
| **`J`** | White Key | B4 (MIDI 71 / 493.88 Hz) |
| **`K`** | White Key | C5 (MIDI 72 / 523.25 Hz) |
| **`1`** | Waveform | Sine Wave |
| **`2`** | Waveform | Square Wave |
| **`3`** | Waveform | Sawtooth Wave |

### Mouse Performance Controls
- **Piano Keys**: Click directly on the virtual 3D keys on screen.
- **Pitch Ribbon**: Click and drag horizontally across the ribbon bar to glide continuously between MIDI 48 and 72.

---

## 📂 Repository File Structure

```text
├── src/                                  # C++ OpenFrameworks Core DSP & App
│   ├── main.cpp                          # Application entry point (1024x768 OpenGL)
│   ├── ofApp.h / ofApp.cpp               # Lifecycle, graphics rendering & input routing
│   ├── WaveType.h                        # Waveform enumeration (SINE, SQUARE, SAW)
│   ├── Oscillator.h                      # Abstract base oscillator class
│   ├── SineOscillator.h                  # Sine wave generator implementation
│   ├── SquareOscillator.h                # Square wave generator implementation
│   ├── SawOscillator.h                   # Sawtooth wave generator implementation
│   ├── Envelope.h / Envelope.cpp         # 4-stage ADSR finite-state machine
│   ├── Voice.h / Voice.cpp               # Monophonic voice unit
│   ├── Synth.h / Synth.cpp               # Polyphonic audio engine & buffer mixer
│   ├── Visualizer.h / Visualizer.cpp     # Oscilloscope & DFT spectrum renderer
│   │
│   ├── audio/                            # Interactive Web Audio Engine (TypeScript)
│   │   ├── Oscillator.ts
│   │   ├── Envelope.ts
│   │   ├── Voice.ts
│   │   ├── SynthEngine.ts
│   │   └── VisualizerEngine.ts
│   │
│   ├── components/                       # Interactive Web User Interface (React)
│   │   ├── Header.tsx
│   │   ├── PianoKeyboard.tsx
│   │   ├── PitchRibbon.tsx
│   │   ├── SynthControls.tsx
│   │   ├── VisualizerCanvas.tsx
│   │   └── ArchitectureModal.tsx
│   │
│   ├── App.tsx                           # Web preview application controller
│   ├── main.tsx                          # Web entry point
│   ├── types.ts                          # Shared data interfaces
│   └── index.css                         # Styling rules
│
├── .gitignore                            # macOS, Windows, Xcode, VS exclusion rules
├── metadata.json                         # Project metadata
├── package.json                          # Web preview build dependencies
└── README.md                             # Academic project documentation
```

---

## 🚀 Build & Installation Guide

### Option 1: Native C++ Build with openFrameworks v0.11.2 (macOS / Xcode)

1. **Prerequisites**: Download and install [openFrameworks v0.11.2](https://openframeworks.cc/download/) for macOS.
2. **Placement**: Clone or copy this repository into your openFrameworks apps directory:
   ```bash
   openFrameworks/apps/myApps/Olgas-Synthesizer-Visualizer/
   ```
3. **Generate Xcode Project**:
   - Open `openFrameworks/projectGenerator/projectGenerator.app`.
   - Click **"Import"** and select the `Olgas-Synthesizer-Visualizer` folder.
   - Click **"Update"** (or **"Generate"**).
4. **Build & Run**:
   - Open `Olgas-Synthesizer-Visualizer.xcodeproj` in **Xcode**.
   - Select the active scheme (e.g. `Olgas-Synthesizer-Visualizer Debug`).
   - Press **`Cmd + R`** or click **Play ▶** to compile and run.

### Option 2: Native C++ Build (Windows / Visual Studio 2022)

1. Place the repository inside `openFrameworks/apps/myApps/Olgas-Synthesizer-Visualizer/`.
2. Open `projectGenerator.exe`, select the project folder, and click **"Update"**.
3. Open `Olgas-Synthesizer-Visualizer.sln` in **Visual Studio 2022**.
4. Set the build configuration to `Debug` / `x64` and press **`F5`** to run.

### Option 3: Web Audio Simulation Preview (Node.js)

1. Ensure Node.js (v18+) is installed.
2. Install dependencies:
   ```bash
   npm install
   ```
3. Launch development server:
   ```bash
   npm run dev
   ```
4. Open `http://localhost:3000` in your web browser.

---

## 🤝 Team Interface Contracts

| Component | Responsible Partner | Method / Signature | Contract Description |
| :--- | :--- | :--- | :--- |
| **Oscillator Base** | Mohammed | `float generateSample()` | Returns normalized floating sample $[-1.0, 1.0]$ |
| **Pitch Control** | Mohammed | `void setFrequency(float hz)` | Updates oscillator pitch dynamically |
| **Voice Binding** | Olga | `void Voice::setOscillator(Oscillator*)` | Binds voice to active waveform oscillator |
| **Audio Buffer Hook** | Olga | `const std::vector<float>& getBuffer()` | Provides read-only audio buffer to Visualizer |
| **Visualizer Hook** | Mohammed | `void Visualizer::update(const std::vector<float>&)` | Ingests latest buffer for oscilloscope & FFT |
| **Sample Rate** | Shared | `44100 Hz` | Global audio sampling standard |

---

## 📜 License

Academic project developed for Coding Camp II. All rights reserved.

