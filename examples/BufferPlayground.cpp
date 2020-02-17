//include functionality of a basic app
#include "example_app.hpp"

//include class if using
#include "pedal/TSine.hpp"
#include "pedal/Buffer.hpp"
//#include <iostream>
//#include "pedal/DebugTool.hpp"
#include "AudioFFT.h"
#include "pedal/BufferPlayer.hpp"
#include <vector>

const size_t fftSize = 512;
std::vector<float> input(fftSize, 0.0f);
std::vector<float> re(audiofft::AudioFFT::ComplexSize(fftSize));
std::vector<float> im(audiofft::AudioFFT::ComplexSize(fftSize));
std::vector<float> output(fftSize);
audiofft::AudioFFT fftOne;
audiofft::AudioFFT fftTwo;
/*
const size_t fftSize = 1024; // Needs to be power of 2!
*
*   std::vector<float> input(fftSize, 0.0f);
*   std::vector<float> re(audiofft::AudioFFT::ComplexSize(fftSize));
*   std::vector<float> im(audiofft::AudioFFT::ComplexSize(fftSize));
*   std::vector<float> output(fftSize);
*
*   audiofft::AudioFFT fft;
*   fft.init(1024);
*   fft.fft(input.data(), re.data(), im.data());
*   fft.ifft(output.data(), re.data(), im.data());
*/
//DebugTool debugger;
Buffer testBuffer(4000.0f);//Initiate buffer with 10 seconds duration
//testBuffer.fillSineSweep();//breaks
BufferPlayer player(&testBuffer);
//========================Audio Callback
void callback(float* out, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {

  //testBuffer.fillSineSweep();//fails here, but delayed.
  bool writeFile = pdlGetTrigger(app, 0);
  if(writeFile){
 //   testBuffer.writeSoundFile("temp");
  }
  player.setPlayMode((PlayMode)pdlGetDropDown(app, 0));
  player.setInterpolatoinMode((InterpolationMode)pdlGetDropDown(app, 1));
  player.setSpeed(pdlGetSlider(app, 0));
  for (unsigned i = 0; i < buffer; i += 1) {//for entire buffer of frames
    //DebugTool::printOncePerBuffer(oscillator.getFrequency(), i);
    float leftSample = 0.0f;
    float rightSample = 0.0f;
    player.update();
    leftSample = player.getSample(0);
    rightSample = player.getSample(1);
  
    out[channel * i] = leftSample;
    out[channel * i + 1] = rightSample;
  }
}
//======================main loop
int main() {
    pdlExampleApp* app = pdlInitExampleApp(callback);
    if (!app) {//if app doesn't succesfully allocate
      return 1;//cancel program, return 1
    }
    pdlSettings::sampleRate = pdlExampleAppGetSamplingRate(app);
    pdlSettings::bufferSize = pdlExampleAppGetBufferSize(app);
    
    fftOne.init(fftSize);
    fftTwo.init(fftSize);
    
    const char* pathToSoundFile = "ding.wav";
    testBuffer.loadSoundFile(pathToSoundFile);//breaks here
    //make an app (a pointer to an app, actually)
    //testBuffer.writeSoundFile("temp");//danger!
    // Add your GUI elements here
    pdlAddSlider(app, 0, "Speed", -2.0f, 16.0f, 1.0f);
    pdlAddToggle(app, 0, "play", false);
    pdlAddTrigger(app, 0, "trigger");
    
    char* modeMenuContent[]{"ONE_SHOT", "LOOP", "PING_PONG"};
    pdlAddDropDown(app, 0, "Mode", modeMenuContent, 3);

    char* interpolationMenuContent[]{"NONE", "LINEAR", "CUBIC"};
    pdlAddDropDown(app, 1, "Interpolation", interpolationMenuContent, 3);
    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}