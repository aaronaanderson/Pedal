#ifndef ImpulseGenerator_hpp
#define ImpulseGenerator_hpp

#include "pdlSettings.hpp"

class ImpulseGenerator{
  public:
  ImpulseGenerator();
  ImpulseGenerator(float initialFrequency);
  ~ImpulseGenerator();

  float generateSample();
  float* generateBlock();
  
  void setFrequency(float newFrequency);
  void setMaskChance(float newMaskChance);
  void setDeviation(float newDeviation);
  
  float getSample();
  float* getBlock();
  float getFrequency();
  float getMaskChance();
  float getDeviation();
  
  private:
  float frequency;
  float maskChance;
  float deviation;//deviation from periodicity
  float currentSample;
  float* currentBlock;
};
#endif