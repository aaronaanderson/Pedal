//incomplete
#ifndef WaveTableSine_hpp
#define WaveTableSine_hpp

#include "stdio.h"

#include "Settings.hpp"
#include "utilities.hpp"
#include "pedal/Interpolation.hpp"
#include <cmath>

#define TABLESIZE 2048//TODO revisit this
//what's the smallest practical table? Calculate the StN ratio for bit depth

namespace pedal{
//======================Define a wavetable template
class SineTable{
  private://class members are private by default; added for clarity
  static SineTable* instance;//store a pointer to an instance of the table
  //The next value is simply 'what frequency would be played if 1 sample of 
  //of the table was played per 1 sample of the audio out.
  float fundamentalFrequency; 
  SineTable();//constructor is private, which is unusual 
  ~SineTable();
  float* table;//storage of the table;
  
  public:
  static SineTable* getInstance();//provide access to the single instance of the table
  float* getTable();//return a pointer to the table
  float getFundamentalFrequency();
  int getTableSize();
};

//======================Play the table defined above
class WaveTableSine{
  public://everything listed after this is public
  WaveTableSine();//constructor, defined in the cpp
  WaveTableSine(float frequency);//option to set frequency on construction
  ~WaveTableSine();//deconstructor (may be needed to free memory)
  float generateSample();//generate and return a single sample

  //"setters"
  void setFrequency(float newFrequency);
  void setPhase(float newPhase);
  void setAmplitude(float newAmplitude);

  //"getters"
  float getFrequency();
  float getPhase();
  float getAmplitude();
  float getSample();
    
  private://everything after this is private (cannot be accessed externally without
  //a "getter" or a "setter"
  //best practice to leave inner workings private

  SineTable* sineTable = SineTable::getInstance();
  float frequency, phase, amplitude;//standard oscillator variables
  float currentSample;//current working sample
  double phaseIncrement;//extra precision necessary 
};
}//end pedal namespace
#endif
