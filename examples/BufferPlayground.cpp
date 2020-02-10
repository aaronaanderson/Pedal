//include functionality of a basic app
#include "example_app.hpp"

//include class if using
#include "pedal/TSine.hpp"
#include "pedal/Buffer.hpp"
//#include <iostream>
//#include "pedal/DebugTool.hpp"


//DebugTool debugger;
Buffer testBuffer(1000.0f);//Initiate buffer with 10 seconds duration
//testBuffer.fillSineSweep();//breaks
int bufferIndex = 0;
//========================Audio Callback
void callback(float* out, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {

  //testBuffer.fillSineSweep();//fails here, but delayed.
  for (unsigned i = 0; i < buffer; i += 1) {//for entire buffer of frames
    //DebugTool::printOncePerBuffer(oscillator.getFrequency(), i);
    float currentSample = 0.0f;
    if(bufferIndex > testBuffer.getSizeInSamples()-1){
      bufferIndex = 0;
    }else{
      currentSample = testBuffer.getSample(bufferIndex);
      bufferIndex++;
    }
    for (unsigned j = 0; j < channel; j += 1) {//for every sample in frame
      out[channel * i + j] = currentSample;
    }
  }
}
//======================main loop
int main() {
    testBuffer.fillSinc(40);//breaks here
    //make an app (a pointer to an app, actually)
    pdlExampleApp* app = pdlInitExampleApp(callback);
    if (!app) {//if app doesn't succesfully allocate
      return 1;//cancel program, return 1
    }
    pdlSettings::sampleRate = pdlExampleAppGetSamplingRate(app);
    pdlSettings::bufferSize = pdlExampleAppGetBufferSize(app);
    
    // Add your GUI elements here
    //pdlAddSlider(app, 0, "frequency", 0.1f, 40.0f, 3.0f);
    //pdlAddToggle(app, 0, "loud", false);
    //pdlAddTrigger(app, 0, "trigger");
    
    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}