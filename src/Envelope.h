#pragma once
#include <algorithm>

enum EnvelopeStage {
    STAGE_OFF,
    STAGE_ATTACK,
    STAGE_DECAY,
    STAGE_SUSTAIN,
    STAGE_RELEASE
};

class Envelope {
public:
    Envelope(float sr = 44100.0f)
        : sampleRate(sr), currentStage(STAGE_OFF), currentLevel(0.0f),
          attackTime(0.05f), decayTime(0.1f), sustainLevel(0.7f), releaseTime(0.3f) {}

    void setSampleRate(float sr) { sampleRate = sr; }

    void setADSR(float a, float d, float s, float r) {
        attackTime   = std::max(0.005f, a);
        decayTime    = std::max(0.005f, d);
        sustainLevel = std::max(0.0f, std::min(1.0f, s));
        releaseTime  = std::max(0.01f, r);
    }

    void triggerAttack() { currentStage = STAGE_ATTACK; }
    void triggerRelease() { if (currentStage != STAGE_OFF) currentStage = STAGE_RELEASE; }

    float process() {
        switch (currentStage) {
            case STAGE_OFF:
                currentLevel = 0.0f;
                break;
            case STAGE_ATTACK: {
                float attackStep = 1.0f / (attackTime * sampleRate);
                currentLevel += attackStep;
                if (currentLevel >= 1.0f) {
                    currentLevel = 1.0f;
                    currentStage = STAGE_DECAY;
                }
                break;
            }
            case STAGE_DECAY: {
                float decayStep = (1.0f - sustainLevel) / (decayTime * sampleRate);
                currentLevel -= decayStep;
                if (currentLevel <= sustainLevel) {
                    currentLevel = sustainLevel;
                    currentStage = STAGE_SUSTAIN;
                }
                break;
            }
            case STAGE_SUSTAIN:
                currentLevel = sustainLevel;
                break;
            case STAGE_RELEASE: {
                float releaseStep = sustainLevel / (releaseTime * sampleRate);
                currentLevel -= releaseStep;
                if (currentLevel <= 0.0001f) {
                    currentLevel = 0.0f;
                    currentStage = STAGE_OFF;
                }
                break;
            }
        }
        return currentLevel;
    }

    bool isActive() const { return currentStage != STAGE_OFF; }
    float getCurrentLevel() const { return currentLevel; }

private:
    float sampleRate;
    EnvelopeStage currentStage;
    float currentLevel;
    float attackTime;
    float decayTime;
    float sustainLevel;
    float releaseTime;
};
