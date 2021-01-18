#include "example_app.hpp"
#include <iostream>

#include "pedal/LowFrequencyOscillator.hpp"
#include "pedal/WTSaw.hpp"
#include "pedal/MoogLadderFilter.hpp"

using namespace pedal;

WTSaw saw;
MoogLadderFilter filter;
LowFrequencyOscillator lfo;
SmoothValue<float> frequency;

void keyboardCallback(int key, bool keyDown){
  switch(key){
    case '1':
      lfo.setWaveShape(LowFrequencyOscillator::WaveShape::Sine);
    break;
    case '2':
      lfo.setWaveShape(LowFrequencyOscillator::WaveShape::Triangle);
    break;
    case '3':
      lfo.setWaveShape(LowFrequencyOscillator::WaveShape::Saw);
    break;
    case '4':
      lfo.setWaveShape(LowFrequencyOscillator::WaveShape::Square);
    break;
  }
}

void audioCallback(float* output, float* input, int bufferSize, int inputChannels, int outputChannels, PedalExampleApp* app){
  frequency.setTarget(pdlGetSlider(app, 0));

  for(int sampleIndex = 0; sampleIndex < bufferSize; sampleIndex++){
    float currentSample = saw.generateSample();
    lfo.setFrequency(frequency.process());
    filter.setFrequency(lfo.generateSample());
    currentSample = filter.processSample(currentSample);
    for(int channelIndex = 0; channelIndex < outputChannels; channelIndex++){
      output[sampleIndex * outputChannels + channelIndex] = currentSample * 0.1f;
    }
  }
}

int main(){
  //Create the application (an audio callback is required here)
  PedalExampleApp* app = pdlInitializeExampleApp(audioCallback, pdlSettings::sampleRate, pdlSettings::bufferSize);
  pdlSetKeyboardCallback(keyboardCallback);
  saw.setFrequency(30.0f);
  lfo.setOutputRange(60.0f, 2000.0f);
  lfo.setFrequency(0.125f);
  filter.setResonance(0.94f);
  pdlAddSlider(app, 0, "LFO frequency", 0.0f, 18.0f, 2.0f);
  pdlStartExampleApp(app);
  //This is the perpetual loop; it will keep going until the window is closed
  while(pdlRunExampleApp(app)){//while the window is still open
    pdlUpdateExampleApp(app);//continue running the app
  }
  //If this point is reached, the application is closing
  pdlDeleteExampleApp(app);
}