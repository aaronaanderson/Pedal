#include "pedal/utilities.hpp"



float pedal::mtof(float midiValue){//midi to frequency
  return 440.0f * (std::pow(2, (midiValue-69)/12.0f));
}
float pedal::midiNoteToPlaySpeed(int midiNoteValue, int normalizeToNote){
  //by default, normalized to midi note 60 
  float distanceFromNormal = (midiNoteValue - normalizeToNote)/12.0f;//normal = 1.0f
  return std::pow(2.0f, distanceFromNormal);
}
/*
template <class T, class U, class Z> 
T clamp(T input, U lowerBound, Z upperBound){
  T result;
  result = min(max(input, lowerBound), upperBound);
  return result;
}
*/
float pedal::rangedRandom(float minimum, float maximum){
  //find an initial random value
  float signedHalfNormal = (std::rand()/float(RAND_MAX))-0.5f;//(range of -0.5, 0.5)
  float range = maximum - minimum;//distance between max and min
  float center = (maximum + minimum) * 0.5f;//center point
  return (signedHalfNormal*range) + center;
}
float pedal::msToSamples(float timeInMS){
  return timeInMS * 0.001f * Settings::sampleRate;
}
float pedal::secondsToSamples(float timeInSeconds){
  return timeInSeconds * Settings::sampleRate;
}
float pedal::samplesToMS(float samples){
  return (samples*1000.0f)/Settings::sampleRate;
}
//amplitudeToDB is in header since templated
//dBToAmplitude is in header since templated

//pan input from left to right, position range -1 to 1
void pedal::panStereo(float input, float position, float* outputFrame){
  //convert to radians
  float theta = clamp(position, -1.0f, 1.0f) * pedal::PI * 0.25f * -1.0f;
  float cosTheta = std::cos(theta);
  float sinTheta = std::sin(theta);
  outputFrame[0] = pedal::SQRT1_2 * (sinTheta - cosTheta) * input;
  outputFrame[1] = pedal::SQRT1_2 * (sinTheta + cosTheta) * input;
}
/* //function unnecessary, will be moved to buffer class
void normalizeBuffer(float* inputBuffer, int bufferSize, bool correctDC = true){
  float highestValue;
  if(correctDC){
    float lowestValue = 100000.0f;//TODO make inf
    highestValue = -100000.0f;//TOOD make -inf
    for(int i = 0; i < bufferSize; i++){
      lowestValue = fmin(inputBuffer[i], lowestValue);
      highestValue = fmax(inputBuffer[i], highestValue);
    }
    float currentCenter = (highestValue - lowestValue) * 0.5;//find the average
    //offsetValue is added to each sample to bring to 0.0
    float offsetValue = -currentCenter;
    //adjust all values to have a currentCenter of 0.0
    for(int i = 0; i < bufferSize; i++){//for each sample in the buffer
      inputBuffer[i] += offsetValue;//addjust uniformly
    }
  }
  if(!correctDC){//scan to find the highest value since that hasn't been done
    highestValue = 0.0f;//TODO this should really be -inf
    //find the largest value in the buffer
    for(int i = 0; i < bufferSize; i++){//for every sample in the buffer
      highestValue = fmax(fabs(inputBuffer[i]), highestValue);
      //the absolute value is taken as the value closest to the boundry,
      //-1 to 1, could be negative or positive, but we don't need the sign, 
      //just 'how close is the closest sample to the boarder?'
    }
  }
  
  //Do some math, largestValue*scalarValue=1.0
  float scalarValue = 1/highestValue;
  //scale the entire buffer uniformly by this scalar value
  for(int i = 0; i < bufferSize; i++){
    inputBuffer[i] *= scalarValue;
  }
}
*/