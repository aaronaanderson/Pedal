#include "pedal/LowPass.hpp"

using namespace pedal;

LowPass::LowPass(float frequency){
  setFrequency(frequency);
  currentSample = 0.0f;
}

//float LowPass::process(float input) is in the header
//because it is inlined

void LowPass::setFrequency(float newFrequency){
  //These coefficient calculations are from Will Pirkle's
  //Designing Audio Effect Plug-ins in C++, pg 165
  float theta = (pedal::PI * 2.0f * newFrequency)/Settings::sampleRate;
  float gamma = 2.0f - cos(theta);
  b = std::sqrt((gamma * gamma) -1.0f) - gamma;
  a = 1.0f + b;
}
float LowPass::getSample(){return currentSample;}