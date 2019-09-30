#ifndef CTEnvelope_hpp
#define CTEnvelope_hpp

#include "math.h"//for fmin and fmax
#include "pdlSettings.hpp"//so we can access sampleRate and bufferSize

class CTEnvelope{//Constant-Time Envelope (linear piece-wise ADSR)
  public:
  CTEnvelope();//default constructor
  //best to have a standard ADSR as an overload constructor
  CTEnvelope(float initialAttack, float initialDecay, float initialSustain, float initialRelease);
  ~CTEnvelope();//deconstructor, used to clear memory if allocated
  
  void setup(float newAttack, float newDecay, float newSustain, float newRelease);
  float generateSample();//calculate and return next sample
  float* generateBlock();//calculate and return next block of samples
  
  enum modes {ADSR=0, ASR, AR};//3 envelope types
  
  //"getters"
  float getAttack();
  float getDecay();
  float getSustain();
  float getRelease();
  float getSample();
  float* getBlock();
  int getCurrentState();
  int getCurrentMode();

  //"setters"
  void setMode(modes newMode);
  void setAttack(float newAttack);
  void setDecay(float newDecay);
  void setSustain(float newSustain);
  void setRelease(float newRelease);
  //=======================================================
  private://best practice to keep inner workings private
  enum states {OFF=0, ATTACK, DECAY, SUSTAIN, RELEASE};
  void calculateIncrement(states whichIncrement);//nead a 'state' variable
  int currentState;//which phase, off-a-d-s-r, is the envelope in
  int currentMode;//which type of envelope is it, adsr,asr, or ar
  float attack, decay, sustain, release;//internal values for these variables
  float attackIncrement, decayIncrement, releaseIncrement;//necessary to calculate next sample
  float currentSample;//current working sample
  float* currentBlock;//(pointer to) current working block of samples
};
#endif