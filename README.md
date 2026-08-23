# Polyphonic Software Synthesizer & Reactive Visualizer
**Team Dolphin** — Coding Camp II: Interactivity (Project 2)  
**Members:** Olga Kazarina (`s-01132`) & Mohammedsaleh Ibrahim (`s-01136`)  
**Framework:** openFrameworks v0.11.2 (C++) & OpenGL

---

## 📑 Table of Contents
1. [Project Overview](#-project-overview)
2. [Key Features & Assignment Requirements](#-key-features--assignment-requirements)
3. [Team Task Division & Collaboration](#-team-task-division--collaboration)
4. [Object-Oriented Architecture (OOP) & Class Relationships](#-object-oriented-architecture-oop--class-relationships)
5. [DSP & Mathematical Foundations](#-dsp--mathematical-foundations)
6. [Interactive Controls & User Manual](#-interactive-controls--user-manual)
7. [Repository Structure](#-repository-structure)
8. [Build & Execution Guide](#-build--execution-guide)
9. [AI Usage Declaration & Sample Prompts](#-ai-usage-declaration--sample-prompts)

---

## 🎯 Project Overview

This project is a real-time, polyphonic digital audio synthesizer and reactive visualizer built with **C++** and **openFrameworks**. Designed as a modular audio instrument, it produces multiple distinct synthesized waveforms, shapes note dynamics using a state-machine ADSR envelope, supports full 8-voice polyphony, and displays dual-mode real-time visual feedback (time-domain oscilloscope and 128-bin Discrete Fourier Transform spectrum analyzer).

In addition to the standalone C++ openFrameworks desktop application, the repository includes a browser-based simulation for testing and architectural inspection.

---

## ✨ Key Features & Assignment Requirements

| Requirement | Implementation Details | Status |
| :--- | :--- | :--- |
| **Minimum 3 Different Sounds** | **Sine Wave** (pure tone), **Square Wave** (odd harmonics), **Sawtooth Wave** (rich bright harmonics). | ✅ Implemented |
| **Clean OOP Structure (≥ 2 relationships)** | **Inheritance** (`Oscillator` $\to$ `Sine`/`Square`/`Saw`), **Composition** (`Synth` $\to$ `Voice` $\to$ `Envelope` & `Oscillator`), and **Association** (`Synth` $\to$ `Visualizer`). | ✅ Implemented |
| **Sound Visualization (Plus)** | Real-time dual-view visualizer: 512-sample **Oscilloscope** (Time Domain) + 128-bin **FFT/DFT Spectrum Analyzer** (Frequency Domain). | ✅ Implemented |
| **Interaction Patterns (Plus)** | **Polyphonic QWERTY Keybed** (A–K, W/E/T/Y/U keys), **3D On-screen Piano**, and **Continuous Pitch Ribbon** (Glissando / continuous frequency sweep). | ✅ Implemented |
| **Envelope Dynamics (Plus)** | 4-stage **ADSR Volume Envelope** (Attack, Decay, Sustain, Release) per voice to eliminate clicking and provide natural instrument decay. | ✅ Implemented |
| **Polyphony** | 8-voice polyphonic note allocator with active voice tracking. | ✅ Implemented |

---

## 👥 Team Task Division & Collaboration

### **Olga Kazarina (`s-01132`) — Core Audio Engine, Voices & ADSR Envelope**
- **`Synth` (`Synth.h`, `Synth.cpp`)**: Master synthesizer class managing the voice bank, master volume, and sound buffer generation via `audioOut(ofSoundBuffer&)`.
- **`Voice` (`Voice.h`, `Voice.cpp`)**: Voice manager responsible for individual note lifecycle, MIDI-to-Hz frequency conversion, and multiplying oscillator samples by envelope gains.
- **`Envelope` (`Envelope.h`)**: 4-stage ADSR finite-state machine with linear attack/decay and zero-sustain safety calculation.
- **Buffer Integration**: Thread-safe extraction of audio frames (`getBuffer()`) for visualization.

### **Mohammedsaleh Ibrahim (`s-01136`) — Oscillators & Real-Time Visualizer**
- **`Oscillator` (`Oscillator.h`)**: Abstract base class defining `generateSample() = 0`, sample rate configuration, and phase accumulation.
- **Waveform Subclasses (`SineOscillator.h`, `SquareOscillator.h`, `SawOscillator.h`)**: Mathematical implementations of trigonometric sine, bipolar square, and linear ramp sawtooth waveforms.
- **`Visualizer` (`Visualizer.h`, `Visualizer.cpp`)**: Dual-mode visualizer rendering the green oscilloscope curve and computing Discrete Fourier Transform (DFT) for frequency spectrum bars.

### **Joint Collaboration**
- Cross-platform build configurations for macOS (`Makefile`, `config.make`, `Project.xcconfig`, `.xcodeproj`) and Windows (`.sln`).
- Interface design and testing for pitch ribbon continuous interaction.
- Project documentation, learning log documentation on Yellowdig, and architecture design.

---

## 🏗️ Object-Oriented Architecture (OOP) & Class Relationships

```text
┌─────────────────────────────────────────┐                   ┌─────────────────────────────────────────┐
│                 Synth                   │─── reads buffer ─>│               Visualizer                │
│        Master Audio Engine Callback     │                   │       Oscilloscope & FFT Spectrum       │
└─────────────────────────────────────────┘                   └─────────────────────────────────────────┘
         │                               │
      composes                        composes
     [8 Voices]                     [Waveform Type]
         │                               │
         ▼                               ▼
┌─────────────────────────────────────────┐                   ┌─────────────────────────────────────────┐
│                 Voice                   │                   │          Oscillator «abstract»          │
│          One Polyphonic Note            │                   │      virtual generateSample() = 0       │
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

### Class Relationships Breakdown:
1. **Inheritance & Polymorphism (`IS-A`)**:
   - `Oscillator` defines a pure virtual interface `virtual float generateSample() = 0`.
   - `SineOscillator`, `SquareOscillator`, and `SawOscillator` inherit from `Oscillator` and provide their respective DSP sample calculations.
   - Allows voices to manipulate any waveform uniformly through the base pointer interface.

2. **Composition (`HAS-A`)**:
   - `Synth` owns and manages an array of 8 `Voice` instances.
   - Each `Voice` owns an `Envelope` object and a dedicated `Oscillator` instance to guarantee independent phase and pitch during polyphonic play.

3. **Association & Loose Coupling (`USES`)**:
   - `Visualizer` receives a const reference to the `Synth` audio buffer (`synth.getBuffer()`) each frame.
   - The visualizer processes and renders audio data without owning or modifying the synthesizer state.

---

## 📐 DSP & Mathematical Foundations

### 1. Phase Accumulation
Every oscillator advances its phase angle per sample:
$$\Delta\theta = \frac{2\pi \cdot f}{f_s}, \quad \theta_{n+1} = (\theta_n + \Delta\theta) \pmod{2\pi}$$

### 2. Frequency Conversion
Converts standard MIDI note numbers ($m \in [0, 127]$) to fundamental frequency in Hertz ($f$):
$$f = 440.0 \times 2^{\frac{m - 69}{12}}$$

### 3. Waveform Equations
- **Sine Wave**: $y(t) = \sin(\theta)$
- **Square Wave**: $y(t) = \begin{cases} 1.0 & \text{if } \theta < \pi \\ -1.0 & \text{if } \theta \ge \pi \end{cases}$
- **Sawtooth Wave**: $y(t) = 1.0 - \frac{\theta}{\pi}$

### 4. ADSR Envelope Stages
- **Attack**: $L_{n+1} = L_n + \frac{1.0}{T_A \cdot f_s}$
- **Decay**: $L_{n+1} = L_n - \frac{1.0 - S}{T_D \cdot f_s}$
- **Sustain**: $L_n = S$ (held constant while active)
- **Release**: $L_{n+1} = L_n - \frac{L_{\text{releaseStart}}}{T_R \cdot f_s}$

### 5. Discrete Fourier Transform (DFT Spectrum)
$$X[k] = \sum_{n=0}^{N-1} x[n] \left( \cos\left(\frac{2\pi k n}{N}\right) - i \sin\left(\frac{2\pi k n}{N}\right) \right)$$
$$\text{Magnitude}[k] = \frac{1}{N}\sqrt{\text{Re}[k]^2 + \text{Im}[k]^2}$$

---

## 🎮 Interactive Controls & User Manual

### Keyboard Key Mapping (C4 to C5 Octave)
| Key | Type | Note | Frequency |
| :---: | :---: | :---: | :---: |
| **`A`** | White | C4 | 261.63 Hz |
| **`W`** | Black | C#4 | 277.18 Hz |
| **`S`** | White | D4 | 293.66 Hz |
| **`E`** | Black | D#4 | 311.13 Hz |
| **`D`** | White | E4 | 329.63 Hz |
| **`F`** | White | F4 | 349.23 Hz |
| **`T`** | Black | F#4 | 369.99 Hz |
| **`G`** | White | G4 | 392.00 Hz |
| **`Y`** | Black | G#4 | 415.30 Hz |
| **`H`** | White | A4 | 440.00 Hz |
| **`U`** | Black | A#4 | 466.16 Hz |
| **`J`** | White | B4 | 493.88 Hz |
| **`K`** | White | C5 | 523.25 Hz |

### Sound / Waveform Selection Keys
- Press **`1`**: Switch to **Sine Wave** (Pure, fundamental tone).
- Press **`2`**: Switch to **Square Wave** (Punchy, hollow tone).
- Press **`3`**: Switch to **Sawtooth Wave** (Bright, aggressive brass-like tone).

### Mouse & Continuous Interaction
- **Piano Keyboard**: Click on any piano key to play and release.
- **Pitch Ribbon**: Click and drag your mouse across the ribbon strip to perform continuous glissando pitch sweeps across the octave range.

---

## 📂 Repository Structure

```text
├── src/                                  # C++ openFrameworks Source Code
│   ├── main.cpp                          # App entry point (OpenGL window setup)
│   ├── ofApp.h / ofApp.cpp               # Event loop, drawing & input routing
│   ├── WaveType.h                        # Waveform enumeration (SINE, SQUARE, SAW)
│   ├── Oscillator.h                      # Abstract base oscillator class
│   ├── SineOscillator.h                  # Sine oscillator implementation
│   ├── SquareOscillator.h                # Square oscillator implementation
│   ├── SawOscillator.h                   # Sawtooth oscillator implementation
│   ├── Envelope.h                        # 4-stage ADSR finite-state machine
│   ├── Voice.h / Voice.cpp               # Polyphonic voice unit
│   ├── Synth.h / Synth.cpp               # Polyphonic synthesizer & audio callback
│   └── Visualizer.h / Visualizer.cpp     # Oscilloscope & DFT spectrum visualizer
│
├── Makefile                              # openFrameworks Linux/macOS build script
├── config.make                           # openFrameworks build flags
├── addons.make                           # Project addons configuration
├── Project.xcconfig                      # Xcode build configuration
├── synthesizer-visualizer.xcodeproj/     # Xcode Project definition
├── .gitignore                            # Standard macOS/Windows/IDE ignore list
└── README.md                             # Project documentation
```

---

## 🚀 Build & Execution Guide

### Option 1: Native openFrameworks Desktop Build (macOS / Xcode)
1. Ensure **openFrameworks v0.11.2** is installed.
2. Place the project inside `openFrameworks/apps/myApps/synthesizer-visualizer/`.
3. Open `synthesizer-visualizer.xcodeproj` in **Xcode**.
4. Select the build target (e.g. `synthesizer-visualizer Debug`) and press **`Cmd + R`** to compile and run.

### Option 2: Native openFrameworks Desktop Build (Windows / Visual Studio)
1. Place the project folder into `openFrameworks/apps/myApps/synthesizer-visualizer/`.
2. Open the openFrameworks **Project Generator**, import the folder, and click **Update**.
3. Open the generated `.sln` in **Visual Studio 2022**, select `Debug` / `x64`, and press **`F5`**.

### Option 3: Command Line (macOS / Linux)
```bash
make
make RunRelease
```

---

## 🤖 AI Usage Declaration & Sample Prompts

In accordance with the course academic integrity guidelines, AI assistance was utilized to support specific software engineering and debugging tasks:

### 1. Purposes for which AI was employed:
- **Code Review & Auditing**: Verifying C++ memory safety, ensuring virtual destructors are properly declared, and validating polyphonic voice lifecycle management.
- **DSP Math Verification**: Checking discrete frequency calculations and phase accumulation boundary conditions.
- **Build Path Normalization**: Removing hardcoded local paths in openFrameworks `Makefile` and `Project.xcconfig` to ensure portable builds for graders.

### 2. Sample Prompts Used:
1. *"In a polyphonic synthesizer where multiple voices share an oscillator pointer, why do notes collide when playing chords, and how should ownership be structured in C++ using OOP?"*
2. *"Review our ADSR Envelope release phase calculation: if sustain is set to 0.0, why does the release step evaluate to zero and how do we calculate release from the current level?"*
3. *"Help configure openFrameworks Makefile and xcconfig paths using relative `OF_ROOT = ../../..` so it compiles cleanly on any grader's machine without hardcoded paths."*

### 3. Declaration:
All core design decisions, architecture modularity, class relationship models, and interface definitions were formulated and implemented by Team Dolphin. All visualizer graphics and DSP algorithms directly serve functional purposes without unrequested decorative clutter.

---

## 📜 License & Academic Integrity
Developed by **Team Dolphin** (Olga Kazarina & Mohammedsaleh Ibrahim) for **Coding Camp II: Interactivity** at German University of Digital Science. All rights reserved.

