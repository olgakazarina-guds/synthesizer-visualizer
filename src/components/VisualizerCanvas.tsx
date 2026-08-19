// ==============================================================================
// VisualizerCanvas.tsx
// Real-time oscilloscope waveform & FFT frequency spectrum rendering canvas.
//
// Features:
// - Matches C++ Visualizer.cpp: 60 FPS update loop.
// - Top half: Time-domain oscilloscope waveform with glow effect.
// - Bottom half: Frequency-domain 128-band FFT spectrum bars with gradient fill.
// ==============================================================================

import React, { useRef, useEffect } from 'react';
import { VisualizerEngine } from '../audio/VisualizerEngine';
import { Activity, BarChart2 } from 'lucide-react';

interface VisualizerCanvasProps {
  visualizer: VisualizerEngine;
  buffer: Float32Array;
  isAudioStarted: boolean;
}

export const VisualizerCanvas: React.FC<VisualizerCanvasProps> = ({
  visualizer,
  buffer,
  isAudioStarted,
}) => {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);

  useEffect(() => {
    let animationFrameId: number;

    const render = () => {
      const canvas = canvasRef.current;
      if (!canvas) return;

      const ctx = canvas.getContext('2d');
      if (!ctx) return;

      // Ensure canvas resolution matches display size
      const dpr = window.devicePixelRatio || 1;
      const rect = canvas.getBoundingClientRect();
      if (canvas.width !== rect.width * dpr || canvas.height !== rect.height * dpr) {
        canvas.width = rect.width * dpr;
        canvas.height = rect.height * dpr;
      }

      ctx.save();
      ctx.scale(dpr, dpr);

      const width = rect.width;
      const height = rect.height;

      // Dark background matching ofBackground(20, 24, 30)
      ctx.fillStyle = '#14181e';
      ctx.fillRect(0, 0, width, height);

      // Update FFT and buffer
      visualizer.update(buffer);

      // Split canvas into top: WAVEFORM and bottom: SPECTRUM
      const padding = 16;
      const availableHeight = height - padding * 3;
      const panelHeight = availableHeight / 2;

      // --- PANEL 1: WAVEFORM ---
      const waveY = padding;
      const waveW = width - padding * 2;
      const waveH = panelHeight;

      // Subdued container background for waveform
      ctx.fillStyle = '#0d1117';
      ctx.strokeStyle = '#1e293b';
      ctx.lineWidth = 1;
      ctx.beginPath();
      ctx.roundRect(padding, waveY, waveW, waveH, 6);
      ctx.fill();
      ctx.stroke();

      visualizer.drawWaveform(ctx, padding + 8, waveY + 8, waveW - 16, waveH - 16);

      // --- PANEL 2: SPECTRUM ---
      const specY = waveY + waveH + padding;
      const specW = width - padding * 2;
      const specH = panelHeight;

      // Subdued container background for spectrum
      ctx.fillStyle = '#0d1117';
      ctx.strokeStyle = '#1e293b';
      ctx.lineWidth = 1;
      ctx.beginPath();
      ctx.roundRect(padding, specY, specW, specH, 6);
      ctx.fill();
      ctx.stroke();

      visualizer.drawSpectrum(ctx, padding + 8, specY + 8, specW - 16, specH - 16);

      ctx.restore();

      animationFrameId = requestAnimationFrame(render);
    };

    animationFrameId = requestAnimationFrame(render);

    return () => {
      cancelAnimationFrame(animationFrameId);
    };
  }, [visualizer, buffer]);

  return (
    <div id="visualizer-container" className="flex flex-col bg-[#0f1318] border border-slate-800 rounded-xl p-4 shadow-lg h-full">
      <div className="flex items-center justify-between pb-3 border-b border-slate-800 mb-2">
        <div className="flex items-center gap-2">
          <Activity className="w-5 h-5 text-emerald-400" />
          <span className="font-mono text-sm font-semibold tracking-wider text-slate-200">
            REAL-TIME OSCILLOSCOPE & FFT
          </span>
        </div>
        <div className="flex items-center gap-4 text-xs font-mono text-slate-400">
          <span className="flex items-center gap-1">
            <span className="w-2 h-2 rounded-full bg-emerald-400"></span> Waveform
          </span>
          <span className="flex items-center gap-1">
            <span className="w-2 h-2 rounded-full bg-indigo-400"></span> 128-Band FFT
          </span>
          {!isAudioStarted && (
            <span className="px-2 py-0.5 rounded bg-amber-500/20 text-amber-300 border border-amber-500/30">
              Click any key to activate audio
            </span>
          )}
        </div>
      </div>

      <div className="relative flex-1 min-h-[300px] w-full">
        {/* Floating Labels matching openFrameworks layout */}
        <div className="absolute top-6 left-6 z-10 pointer-events-none flex items-center gap-1.5 font-mono text-xs font-bold text-slate-400 tracking-wider">
          <Activity className="w-3.5 h-3.5 text-emerald-400" />
          WAVEFORM (TIME DOMAIN)
        </div>

        <div className="absolute top-[53%] left-6 z-10 pointer-events-none flex items-center gap-1.5 font-mono text-xs font-bold text-slate-400 tracking-wider">
          <BarChart2 className="w-3.5 h-3.5 text-indigo-400" />
          SPECTRUM (DFT / FFT MAGNITUDE)
        </div>

        <canvas
          id="synth-visualizer-canvas"
          ref={canvasRef}
          className="w-full h-full rounded-lg"
        />
      </div>
    </div>
  );
};

