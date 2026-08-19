#pragma once
#include <algorithm>

// ==============================================================================
// Envelope.h
// ADSR Volume Envelope Generator (Attack, Decay, Sustain, Release).
//
// Role in project architecture (Olga / Left UML):
// - Shapes the volume of each note over time to prevent sudden audio clicks.
// - Implements a finite state machine:
//     1. STAGE_OFF: Note is silent (level = 0.0).
//     2. STAGE_ATTACK: Ramps from 0.0 up to 1.0 (peak volume).
//     3. STAGE_DECAY: Drops from 1.0 down to sustain level.
//     4. STAGE_SUSTAIN: Stays constant while the key is held down.
//     5. STAGE_RELEASE: Fades from sustain level down to 0.0 when key is released.
// ==============================================================================

enum EnvelopeStage {
    STAGE_OFF,
    STAGE_ATTACK,
    STAGE_DECAY,
    STAGE_SUSTAIN,
    STAGE_RELEASE
};

class Envelope {
public:
    // Constructor: sets default time intervals and starts in the OFF stage
    Envelope(float sr = 44100.0f)
        : sampleRate(sr), currentStage(STAGE_OFF), currentLevel(0.0f),
          attackTime(0.05f), decayTime(0.1f), sustainLevel(0.7f), releaseTime(0.3f) {}

    // Update sample rate if audio device config changes
    void setSampleRate(float sr) { sampleRate = sr; }

    // Configure the ADSR parameters (with safe minimum clamp values)
    void setADSR(float a, float d, float s, float r) {
        attackTime   = std::max(0.005f, a);               // Attack duration (seconds)
        decayTime    = std::max(0.005f, d);               // Decay duration (seconds)
        sustainLevel = std::max(0.0f, std::min(1.0f, s)); // Sustain ratio (0.0 to 1.0)
        releaseTime  = std::max(0.01f, r);                // Release duration (seconds)
    }

    // Called when a piano key is pressed down
    void triggerAttack() { 
        currentStage = STAGE_ATTACK; 
    }

    // Called when a piano key is released
    void triggerRelease() { 
        if (currentStage != STAGE_OFF) {
            currentStage = STAGE_RELEASE; 
        }
    }

    // Advances the envelope state by 1 sample and returns the current gain multiplier (0.0 to 1.0)
    float process() {
        switch (currentStage) {
            case STAGE_OFF:
                currentLevel = 0.0f;
                break;

            case STAGE_ATTACK: {
                // Step size needed to reach 1.0 in attackTime seconds
                float attackStep = 1.0f / (attackTime * sampleRate);
                currentLevel += attackStep;
                if (currentLevel >= 1.0f) {
                    currentLevel = 1.0f;
                    currentStage = STAGE_DECAY; // Move to Decay once peak is reached
                }
                break;
            }

            case STAGE_DECAY: {
                // Step size needed to drop from 1.0 to sustainLevel in decayTime seconds
                float decayStep = (1.0f - sustainLevel) / (decayTime * sampleRate);
                currentLevel -= decayStep;
                if (currentLevel <= sustainLevel) {
                    currentLevel = sustainLevel;
                    currentStage = STAGE_SUSTAIN; // Hold at sustain level
                }
                break;
            }

            case STAGE_SUSTAIN:
                // Remain at the sustain volume while key is held
                currentLevel = sustainLevel;
                break;

            case STAGE_RELEASE: {
                // Step size needed to drop from sustainLevel to 0.0 in releaseTime seconds
                float releaseStep = sustainLevel / (releaseTime * sampleRate);
                currentLevel -= releaseStep;
                if (currentLevel <= 0.0001f) {
                    currentLevel = 0.0f;
                    currentStage = STAGE_OFF; // Note finished playing
                }
                break;
            }
        }
        return currentLevel;
    }

    // Checks if the envelope is still producing sound
    bool isActive() const { return currentStage != STAGE_OFF; }

    // Read current amplitude level
    float getCurrentLevel() const { return currentLevel; }

private:
    float sampleRate;
    EnvelopeStage currentStage;
    float currentLevel;    // Current envelope gain (0.0 to 1.0)
    float attackTime;      // Attack time in seconds
    float decayTime;       // Decay time in seconds
    float sustainLevel;    // Sustain level (0.0 to 1.0)
    float releaseTime;     // Release time in seconds
};

