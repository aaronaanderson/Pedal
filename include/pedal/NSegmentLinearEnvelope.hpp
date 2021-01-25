#ifndef NSegmentLinearEnvelope_hpp
#define NSegmentLinearEnvelope_hpp

#include <vector>
#include <cmath>
#include "pedal/Settings.hpp"

namespace pedal{
class NSegmentLinearEnvelope{
  public:
  struct Segment{
    float targetValue;//value reached at the end of this segment
    double increment;//will vary based on which segment precedes it
    float durationMS;//how long, in miliseconds, does this segment last
  };
  NSegmentLinearEnvelope();

  //alternate 
  float generateSample();


  void appendSegment(float targetValue, float durationMS);
  void insertSegment(int index, float targetValue, float durationMS);
  void moveSegment(int originatingIndex, int destinationIndex);
  void removeSegment(int segmentIndex);
  void clearSegments();
  void setTrigger(bool newTrigger);
  void setInitialValue(float initialValue);
  bool getTrigger();
  float getSample();

  private:
  void incrementSegment();
  float currentSample;
  bool trigger;
  bool active;
  std::vector<Segment> segments;
  int currentSegment;
  float initialValue;
};
}//end pedal namespace

#endif