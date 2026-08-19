// ==============================================================================
// VisualizerEngine.ts
// Real-time Canvas Visualizer rendering Oscilloscope & DFT Spectrum.
//
// Role in project architecture (Mohammed / Right UML):
// - Renders green time-domain oscilloscope waveform.
// - Performs Discrete Fourier Transform (DFT) to render frequency magnitude bars.
// ==============================================================================

export class VisualizerEngine {
  private waveform: Float32Array;
  private spectrum: Float32Array;
  private fftSize: number;

  constructor(fftSize: number = 256) {
    this.fftSize = fftSize;
    this.waveform = new Float32Array(fftSize);
    this.spectrum = new Float32Array(fftSize / 2);
  }

  // Configure FFT size
  public setup(size: number = 256): void {
    this.fftSize = size;
    this.waveform = new Float32Array(size);
    this.spectrum = new Float32Array(size / 2);
  }

  // Update buffer with latest audio samples
  public update(samples: Float32Array): void {
    if (!samples || samples.length === 0) return;
    
    // Copy samples
    if (this.waveform.length !== samples.length) {
      this.waveform = new Float32Array(samples.length);
    }
    this.waveform.set(samples);

    this.computeSpectrum();
  }

  // Compute Discrete Fourier Transform (DFT) to convert Time Domain -> Frequency Domain
  private computeSpectrum(): void {
    if (this.waveform.length < this.fftSize) return;

    // Discrete Fourier Transform over the first fftSize samples
    // (exact mathematical algorithm from C++ Visualizer.cpp)
    const halfSize = this.fftSize / 2;
    for (let k = 0; k < halfSize; ++k) {
      let re = 0.0;
      let im = 0.0;
      for (let n = 0; n < this.fftSize; n++) {
        const angle = (2.0 * Math.PI * k * n) / this.fftSize;
        re += this.waveform[n] * Math.cos(angle);
        im -= this.waveform[n] * Math.sin(angle);
      }
      this.spectrum[k] = Math.sqrt(re * re + im * im) / this.fftSize;
    }
  }

  public getWaveform(): Float32Array {
    return this.waveform;
  }

  public getSpectrum(): Float32Array {
    return this.spectrum;
  }

  // Draw oscilloscope waveform on HTML5 canvas
  public drawWaveform(
    ctx: CanvasRenderingContext2D,
    x: number,
    y: number,
    w: number,
    h: number
  ): void {
    ctx.save();

    // Center baseline in neutral gray
    ctx.strokeStyle = 'rgb(120, 120, 120)';
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.moveTo(x, y + h / 2);
    ctx.lineTo(x + w, y + h / 2);
    ctx.stroke();

    // Waveform line in emerald/mint: rgb(80, 220, 160) matching C++ ofApp
    ctx.strokeStyle = 'rgb(80, 220, 160)';
    ctx.lineWidth = 2;
    ctx.beginPath();

    if (this.waveform.length > 0) {
      const len = this.waveform.length;
      for (let i = 0; i < len; ++i) {
        const px = x + (i / (len - 1)) * w;
        const py = y + h / 2 + this.waveform[i] * (h / 2);
        if (i === 0) {
          ctx.moveTo(px, py);
        } else {
          ctx.lineTo(px, py);
        }
      }
    } else {
      ctx.moveTo(x, y + h / 2);
      ctx.lineTo(x + w, y + h / 2);
    }
    ctx.stroke();

    ctx.restore();
  }

  // Draw frequency spectrum bars on HTML5 canvas
  public drawSpectrum(
    ctx: CanvasRenderingContext2D,
    x: number,
    y: number,
    w: number,
    h: number
  ): void {
    ctx.save();

    const specLen = this.spectrum.length;
    if (specLen > 0) {
      const barW = w / specLen;
      for (let i = 0; i < specLen; ++i) {
        const mag = Math.max(0.0, Math.min(1.0, this.spectrum[i] * 6.0));
        const barH = mag * h;

        // Color formula from C++ Visualizer.cpp:
        // ofSetColor(60 + mag * 195, 120, 220 - mag * 120);
        const r = Math.round(60 + mag * 195);
        const g = 120;
        const b = Math.round(220 - mag * 120);

        ctx.fillStyle = `rgb(${r}, ${g}, ${b})`;
        ctx.fillRect(x + i * barW, y + h - barH, Math.max(1, barW - 1), barH);
      }
    }

    ctx.restore();
  }
}

