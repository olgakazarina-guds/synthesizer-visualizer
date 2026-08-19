# OpenFrameworks Software Synthesizer & Visualizer

A real-time C++ polyphonic software synthesizer and reactive visualizer built with **openFrameworks v0.11.2** for **Coding Camp II (Project 2)**.

---

## 👥 Authors & Task Division

- **Olga (Left UML Architecture)** — Audio Engine, Voice & Envelope
  - `Synth`: Core audio engine container and `audioOut(ofSoundBuffer &buffer)` stream callback.
  - `Voice`: Polyphonic note management and oscillator binding.
  - `Envelope`: ADSR envelope generator shaping volume over time.
  - Variable & Interface Contracts.

- **Mohammed (Right UML Architecture)** — Oscillator DSP & Visualizer
  - `Oscillator`: Abstract base class with pure virtual `generateSample()` interface.
  - Waveform Subclasses: `SineOscillator`, `SquareOscillator`, and `SawOscillator` (Inheritance).
  - `Visualizer`: Real-time oscilloscope waveform & FFT frequency spectrum rendering.

- **Joint / Shared Collaboration**
  - Continuous integration and interface synchronization (`generateSample()`, `sampleRate`, `frequency`, `triggerNote()`, `setADSR()`).
  - Comprehensive Lab Report, user manual, and performance testing.
  - Continuous progress logging on Yellowdig.

---

## 🏗️ Object-Oriented Architecture (OOP)

The application models sound synthesis strictly following our design UML class diagram:

```text
┌──────────────────────────────────────┐                   ┌──────────────────────────────────────┐
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
└──────────────────────────────────────┘
```

### Key OOP Relationships
1. **Inheritance (`is-a` / ▷)**:
   - `Oscillator` is an abstract base class defining `virtual float generateSample() = 0`.
   - `SineOscillator`, `SquareOscillator`, and `SawOscillator` inherit from `Oscillator` and implement specific DSP mathematical algorithms.
2. **Composition (`has-a` / ◆)**:
   - `Synth` composes multiple `Voice` instances and `Oscillator` pointers.
   - Each `Voice` composes an `Envelope` to shape the note's ADSR amplitude over time.
3. **Association (`uses` / ⇢)**:
   - `Visualizer` reads audio buffer data from `Synth` via `getBuffer()` to render real-time waveforms.

---

## 🤝 Team Interface Contract

| Component | Responsibility | Agreed Interface / Method | Notes |
| :--- | :--- | :--- | :--- |
| **Oscillator Base** | Mohammedsaleh | `float generateSample()` | Returns sample in `[-1.0, 1.0]` |
| **Pitch Control** | Mohammedsaleh | `void setFrequency(float hz)` | Updates oscillator pitch |
| **Voice Binding** | Olga | `Voice::setOscillator(Oscillator*)` | Voice holds base pointer |
| **Audio Buffer Hook** | Olga | `const std::vector<float>& getBuffer()` | Returns `[-1.0, 1.0]` buffer for Visualizer |
| **Visualizer Hook** | Mohammedsaleh | `Visualizer::setSynth(Synth*)` | Visualizer reads from Synth pointer |
| **Sample Rate** | Shared | `44100` Hz | Shared global rate |

---

## 🎹 Controls

- **Keys `[A, S, D, F, G, H, J, K]`**: Play C4 to C5 musical notes.
- **Keys `[1, 2, 3]`**: Switch waveform type (`1` = Sine, `2` = Square, `3` = Saw).

---

## 📂 Repository Structure

- **`cpp_source/`**: Pure **C++ / openFrameworks v0.11.2** source code for Xcode (macOS) and Visual Studio 2022 (Windows). Use these files for your project compilation and LMS submission.
- **`src/`**: Interactive TypeScript / Web Audio mirror for the live Google AI Studio web preview.

---

## 🚀 How to Build & Run (C++ openFrameworks)

1. Copy the contents of `cpp_source/` into `openFrameworks/apps/myApps/Synthesizer/src/`.
2. Open the **openFrameworks Project Generator** (`openFrameworks/projectGenerator/`).
3. Set project path to `apps/myApps/Synthesizer` and click **Update** (or **Generate**).
4. Open the generated project in:
   - **macOS**: `Synthesizer.xcodeproj` in **Xcode** (click Run ▶).
   - **Windows**: `Synthesizer.sln` in **Visual Studio 2022** (press F5).
