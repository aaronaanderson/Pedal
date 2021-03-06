#include "exampleAppSource/example_app.hpp"//include functionality of a basic app

//include class if using
#include "pedal/Settings.hpp"
#include "pedal/Delay.hpp"
#include "pedal/utilities.hpp"
#include "pedal/SmoothValue.hpp"

using namespace pedal;

Delay delay;//feedback delay
SmoothValue<float> delayTime;//smooth changes in delay time

//========================Audio Callback
void callback(float* output, float* input, int bufferSize, int inputChannels, int outputChannels, app::PedalExampleApp* app) {
  
  delay.setFeedback(app::getSlider(app, 0));//set feedback amplitude from 0th slider
  delayTime.setTarget(app::getSlider(app, 1));//tell smooth value to target 1st slider value

  for (unsigned i = 0; i < bufferSize; i += 1) {//for entire buffer of frames
    float liveInputSample = input[i * inputChannels];//get the input sample (on the 0th channel)
    delayTime.process();//advance SmoothValue
    //set delay time per-sample
    delay.setDelayTime(delayTime.getCurrentValue());
    delay.insertSample(liveInputSample);//advance the delay object with the input
    float currentSample = delay.getSample();//collect the output

    for (unsigned j = 0; j < outputChannels; j += 1) {//for every sample in frame
      output[outputChannels * i + j] = currentSample * 0.1f;//deliver output to every channel
    }
  }
}
//======================main loop
int main() {
  //make an app (a pointer to an app, actually)
  app::PedalExampleApp* app = app::initializeExampleApp(callback);
  if (!app) {//if app doesn't succesfully allocate
    return 1;//cancel program, return 1
  }
  Settings::sampleRate = getSampleRate(app);
  Settings::bufferSize = getBufferSize(app);
  delayTime.setTime(500.0f);
  // Add your GUI elements here
  app::addSlider(app, 0, "feedback", 0.0f, 0.99f, 0.4f);
  app::addSlider(app, 1, "delay time(ms)", 0.0f, 2000.0f, 500.0f);
  //begin the app--------
  app::startApp(app);
  //Settings::sampleRate = app->sampling_rate;
  while (app::shouldContinue(app)) {//run forever
    app::update(app);//run the application
  }
  //the application has stopped running, 
  app::freeMemory(app);//free the app from memory
}