#include "pedal/CREnvelope.hpp"

using namespace pedal;

CREnvelope::CREnvelope(){
  //offset to get the more linear part of the curve
  attack.curveOvershoot = std::exp(-1.5);
  //attackCurveOvershoot = 0.9999f;//try this
  attack.curveOffset = -std::log((1.0f + attack.curveOvershoot)/
                                 attack.curveOvershoot);
  //long release curve
  decay.curveOvershoot = std::exp(-4.5);
  //decayCurveOvershoot = std::exp(-11.5);//try this (linear in dB)
  decay.curveOffset = -std::log((1.0f + decay.curveOvershoot)/
                                decay.curveOvershoot);
  release.curveOvershoot = decay.curveOvershoot;
  release.curveOffset = decay.curveOffset;
  setMode(Mode::ADSR);
  setAttackTime(100.0f);
  setDecayTime(40.0f);
  setSustainLevel(0.7f);
  setReleaseTime(800.0f);
  setHoldTime(0.0f);
  trigger = false;
  holdSampleCount = 0;
}
float CREnvelope::generateSample(){
  switch(currentMode){
    case Mode::ADSR://Attack, Decay, Sustain, Release
      switch(currentState){
        case OFF:
        //do nothing
        break;
        case ATTACK:
          currentSample = currentSample * attack.curveCoefficient + attack.offset;
          if(currentSample >= 1.0f || attack.timeInSamples == 0.0f){
            currentState = DECAY;
          }
        break;
        case DECAY:
          currentSample = currentSample * decay.curveCoefficient + decay.offset;
          if(currentSample <= sustainLevel || decay.timeInSamples == 0.0f){
            currentSample = sustainLevel;//adjust if needed
            currentState = SUSTAIN;
          }
        break;
        case SUSTAIN:
          currentSample = sustainLevel;
        break;
        case RELEASE:
          currentSample = currentSample * release.curveCoefficient + release.offset;
          if(currentSample <= 0.0f || release.timeInSamples == 0.0f){
            currentSample = 0.0f;
            currentState = OFF;
          }
        break;
      }
    break;
    case Mode::AHDSR://Attack, Hold, Decay, Sustain, Release
      switch(currentState){
        case OFF:
        //do nothing
        break;
        case ATTACK:
          currentSample = currentSample * attack.curveCoefficient + attack.offset;
          if(currentSample >= 1.0f || attack.timeInSamples == 0.0f){
            currentState = HOLD;
          }
        break;
        case HOLD:
          if(holdSampleCount > holdTimeInSamples){
            currentState = DECAY;
            holdSampleCount = 0;//reset hold for next round
          }else{
            holdSampleCount++;
          }
        break;
        case DECAY:
          currentSample = currentSample * decay.curveCoefficient + decay.offset;
          if(currentSample <= sustainLevel || decay.timeInSamples == 0.0f){
            currentSample = sustainLevel;//adjust if needed
            currentState = SUSTAIN;
          }
        break;
        case SUSTAIN:
          currentSample = sustainLevel;
        break;
        case RELEASE:
          currentSample = currentSample * release.curveCoefficient + release.offset;
          if(currentSample <= 0.0f || release.timeInSamples == 0.0f){
            currentSample = 0.0f;
            currentState = OFF;
          }
        break;
      }
    break;
    case Mode::AHR://Attack, Hold, Release
      switch(currentState){
        case OFF:
        //do nothing
        break;
        case ATTACK:
          currentSample = currentSample * attack.curveCoefficient + attack.offset;
          if(currentSample >= 1.0f || attack.timeInSamples == 0.0f){
            currentState = HOLD;
          }
        break;
        case HOLD:
          if(holdSampleCount > holdTimeInSamples){
            currentState = RELEASE;
            holdSampleCount = 0;//reset hold for next round
          }else{
            holdSampleCount++;
          }
        break;
        case RELEASE:
          currentSample = currentSample * release.curveCoefficient + release.offset;
          if(currentSample <= 0.0f || release.timeInSamples == 0.0f){
            currentSample = 0.0f;
            currentState = OFF;
            trigger = false;
          }
        break;//end of currentState switch
      }
    break;
    case Mode::AR://Attack, Release
      switch(currentState){
        case OFF:
        //do nothing
        break;
        case ATTACK:
          currentSample = currentSample * attack.curveCoefficient + attack.offset;
          if(currentSample >= 1.0f || attack.timeInSamples == 0.0f){
            currentState = RELEASE;
          }
        break;
        case RELEASE:
          currentSample = currentSample * release.curveCoefficient + release.offset;
          if(currentSample <= 0.0f || release.timeInSamples == 0.0f){
            currentSample = 0.0f;
            currentState = OFF;
            trigger = false;
          }
        break;//end of currentState switch
      }
    break;//end of currentMode switch
  }
  return currentSample;
}
void CREnvelope::setTrigger(bool newTrigger){
  //re-trigger if last trigger was false, but new trigger is true
  if(newTrigger == true && trigger == false){
    currentState = ATTACK;
    holdSampleCount = 0;

  }else if(newTrigger == false && trigger == true){
    currentState = RELEASE;
  }
  //assign the new trigger for next call
  if(currentMode == Mode::AHR || currentMode == Mode::AR){
    trigger = false;
  }else{
    trigger = newTrigger;
  }
}
void CREnvelope::setMode(Mode newMode){currentMode = newMode;}
void CREnvelope::setAttackTime(float newAttackTime){
  attack.timeInMS = std::max(newAttackTime, 0.0f);
  calculateAttackCurve(attack.timeInMS);
}
void CREnvelope::setDecayTime(float newDecayTime){
  decay.timeInMS = std::max(newDecayTime, 0.0f);
  calculateDecayCurve(decay.timeInMS);
}
void CREnvelope::setSustainLevel(float newSustainLevel){
  sustainLevel = clamp(newSustainLevel, 0.0f, 1.0f);
  calculateDecayCurve(decay.timeInMS);
}
void CREnvelope::setReleaseTime(float newReleaseTime){
  release.timeInMS = std::max(newReleaseTime, 0.0f);
  calculateReleaseCurve(release.timeInMS);
}
void CREnvelope::setHoldTime(float newHoldTime){
  holdTimeInMS = std::max(newHoldTime, 0.0f);
  holdTimeInSamples = msToSamples(holdTimeInMS);
}

float CREnvelope::getSample(){return currentState;}
CREnvelope::Mode CREnvelope::getCurrentMode(){return currentMode;}
float CREnvelope::getAttackTime(){return attack.timeInMS;}
float CREnvelope::getDecayTime(){return decay.timeInMS;}
float CREnvelope::getSustainLevel(){return sustainLevel;}
float CREnvelope::getReleaseTime(){return release.timeInMS;}
bool CREnvelope::getTrigger(){return trigger;}
bool CREnvelope::isBusy(){
  if(currentState = OFF){
    return false;
  }
  return true;
}