
#include "TSaw.hpp"

//constructors and deconstructors
//=========================================================
pdlTSaw::pdlTSaw(){//default constructor
  setFrequency(440);//default frequency is 440
  setPhase(0.0);
  setAmplitude(1.0);
}

pdlTSaw::pdlTSaw(float frequency){//override constructor
  setFrequency(frequency);
  setPhase(0.0);
  setAmplitude(1.0);
}

pdlTSaw::~pdlTSaw(){//deconstructor (needed to be explicit if freeing memory)
  if(currentBlock != nullptr){//if space was allocated for current
    delete currentBlock;//free the memory
  }
}

//primary mechanics of class
//=========================================================
float pdlTSaw::generateSample(){//return a float even if you don't use it
  currentSample = generateNextSample();//defined in header (b/c inlined function)
  return currentSample;
}

float* pdlTSaw::generateBlock(){//it is best to do all 
  //calculations in a row if possible. This keeps the memory from 
  //jumping around looking for data

  if(currentBlock != nullptr){//if we don't have a local currentBlock yet, 
    currentBlock = new float[pdlSettings::bufferSize];//create a new array of floats
  }

  for(int i = 0; i < pdlSettings::bufferSize; ++i){//for every sample in the buffer
    currentBlock[i] = generateNextSample();
  }
  return currentBlock;//returns pointer to the begining of this block
}
//Getters and setters
//=========================================================
void pdlTSaw::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = (frequency * 2.0)/pdlSettings::sampleRate;
}
void pdlTSaw::setPhase(float newPhase){//set phase (0 - 2PI)
  //convert the standard 0-2PI range to -1 to 1 to make calculations cheap
  phase = fmod(newPhase, 2.0 * M_PI);//make sure phase is in the 0-twopi range
  phase -= M_PI;//now the range is (-pi, pi);
  phase /= M_PI;//now the phase is (-1, 1);
}
void pdlTSaw::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float pdlTSaw::getFrequency(){return frequency;}
float pdlTSaw::getAmplitude(){return amplitude;}
float pdlTSaw::getSample(){return currentSample;}
float* pdlTSaw::getBlock(){return currentBlock;}