#include "pedal/LowPass.hpp"

LowPass::LowPass(float frequency){
  setFrequency(frequency);
  currentSample = 0.0f;
}

//float LowPass::process(float input) is in the header
//because it is inlined

void LowPass::setFrequency(float newFrequency){
  float theta = (M_PI_2 * newFrequency)/pdlSettings::sampleRate;
  float gamma = 2.0f - cos(theta);
  b = sqrt((gamma * gamma) -1.0f) - gamma;
  a = 1.0f + b;
  std::cout << b << " " << a << std::endl;
}
float LowPass::getSample(){return currentSample;}