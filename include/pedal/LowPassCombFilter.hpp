#ifndef LowPassCombFilter_hpp
#define LowPassCombFilter_hpp

#include "Constants.hpp"
#include "CircularBuffer.hpp"
#include "LowPass.hpp"

/*
This is the same as the comb filter, 
except for a lowpass filter in the 
feedback loop. Useful for digital 
reverbs.
*/
namespace pedal{
class LowPassCombFilter{
  public:
  LowPassCombFilter(float maxDelay = 1000.0f, float initialFrequency = 1000.0f);

  float process(float input);
  float getFeedBackDelayTime();
  float getFeedBackGain();
  float getMaxDelayTime();
  float getFilterFrequency();
  void setFilterFrequency(float newFrequency);
  void setDelayTime(float newFBDelayTime);
  void setDelayByFrequency(float frequency);
  void setFeedBackGain(float newFBGain);
  void setMaxDelayTime(float newMaxDelay);
  private:
  float currentSample;
  CircularBuffer delayLine;
  LowPass filter;
  float filterFrequency;
  float delayTime;
  float maxDelayTime = 1000.0f;
  float feedBackGain;
};
}//end pedal namespace
#endif 