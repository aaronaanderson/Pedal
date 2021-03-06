#ifndef CurvedEnvelope_hpp
#define CurvedEnvelope_hpp

#include <cmath>
#include <algorithm>
#include "pedal/Settings.hpp"
#include "pedal/utilities.hpp"

#include "pedal/DebugTool.hpp"

namespace pedal{
//Make a container for segments
struct CurvedSegment{
  float timeInMS;
  float curveCoefficient;
  float curveOffset;
  float timeInSamples;
  float curveOvershoot;
  float offset;
};

class CurvedEnvelope{
  public:
  CurvedEnvelope();
  float generateSample();
  void setTrigger(bool newTrigger);
  enum class Mode{
    ADSR, 
    AHDSR, 
    AR, 
    AHR
  };
  void setMode(Mode newMode);
  void setAttackTime(float newAttackTime);
  void setDecayTime(float newDecayTime);
  void setSustainLevel(float newSustainLevel);
  void setReleaseTime(float newReleaseTime);
  void setHoldTime(float newHoldTime);//applicable to AHDSR and AHR Mode
  void setLegato(float newLegato);//
  float getSample();
  Mode getCurrentMode();
  float getAttackTime();
  float getDecayTime();
  float getSustainLevel();
  float getReleaseTime();
  bool getTrigger();
  bool isBusy();

  private:
  float currentSample;
  enum State {OFF, ATTACK, DECAY, SUSTAIN, RELEASE, HOLD};
  inline void calculateAttackCurve(float newAttackTime);
  inline void calculateDecayCurve(float newDecayTime);
  inline void calculateReleaseCurve(float newReleaseTime);
  int currentState;
  Mode currentMode;
  bool trigger;
  CurvedSegment attack;
  CurvedSegment decay;
  float sustainLevel;
  CurvedSegment release;
  int holdTimeInSamples;
  float holdTimeInMS;
  int holdSampleCount;
  DebugTool debugTool;
};

inline void CurvedEnvelope::calculateAttackCurve(float newAttackTime){
  attack.timeInSamples = Settings::sampleRate * newAttackTime * 0.001f;
  attack.curveCoefficient = std::exp(attack.curveOffset/
                                    attack.timeInSamples);
  attack.offset = (1.0f + attack.curveOvershoot) * 
                 (1.0f - attack.curveCoefficient);
}
inline void CurvedEnvelope::calculateDecayCurve(float newDecayTime){
  decay.timeInSamples = Settings::sampleRate * newDecayTime * 0.001f;
  decay.curveCoefficient = std::exp(decay.curveOffset/ 
                                   decay.timeInSamples);
  decay.offset = (sustainLevel - decay.curveOvershoot) *
                (1.0f - decay.curveCoefficient);
}
inline void CurvedEnvelope::calculateReleaseCurve(float newReleaseTime){
  release.timeInSamples = Settings::sampleRate * newReleaseTime * 0.001f;
  release.curveCoefficient = std::exp(release.curveOffset/
                                     release.timeInSamples);
  release.offset = -release.curveOvershoot * (1.0f - release.curveCoefficient);
}
}//end pedal namespace
#endif