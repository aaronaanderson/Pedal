#include "pedal/TSine.hpp"

using namespace pedal;

//constructors and deconstructors
//=========================================================
TSine::TSine(){//default constructor
  setFrequency(440);
  setPhase(0.0);
  setAmplitude(1.0);
}

TSine::TSine(float frequency){//override constructor
  setFrequency(frequency);
  setPhase(0.0);
  setAmplitude(1.0);
}

TSine::~TSine(){}//when object is deleted

//primary mechanics of class
//=========================================================
float TSine::generateSample(){//generate and return single sample
  currentSample = std::sin(phase) * amplitude;//calculate single sample
  phase += (float)phaseIncrement;//increment phase for the next sample
  phase = std::fmod(phase, pedal::TWOPI);
  return currentSample;//return a copy
}
//expects input phase in the range of 0 to TWOPI
float TSine::generateSample(float inputPhase){//generate and return single sample
  setPhase(inputPhase);
  currentSample = std::sin(phase) * amplitude;//calculate single sample
  return currentSample;//return a copy
}
//Getters and setters
//=========================================================
void TSine::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = (frequency * pedal::TWOPI)/pdlSettings::sampleRate;//*see notes on bottom
}
void TSine::setPhase(float newPhase){phase = newPhase;}
void TSine::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float TSine::getFrequency(){return frequency;}
float TSine::getAmplitude(){return amplitude;}
float TSine::getSample(){return currentSample;}

/*
*Why is a phase increment needed? 

For a 1.0 Hz sinusoid, one could simply calculate the samples as a function of time
sin(time * TWO_PI);
For every second, the sinusoid will make a full rotation (phase is 0 to two_pi; at one
second, sin(1.0 * TWO_PI) is the same as sin(0.0 * TWO_PI)). Replace that 1.0 with 
whatever frequency you desire and it may seem like it should work. 

However, in digital audio samples are not calculated as needed, but are calculated slightly ahead
of time in a group of samples. This group of samples is often called a buffer, a block, or 
in the case of MaxMSP, a "signal vector". Since we have discretized time into samples, and 
we know how many samples exist per second AND how many samples we have to calculate in one group, 
we can calculate how much further the phase must travel in the next sample. This value is called
phaseIncrement.
*/