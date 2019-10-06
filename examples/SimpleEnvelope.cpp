//include functionality of a basic app
#include "example_app.hpp"

//include class if using
#include "pedal/TSine.hpp"
#include "pedal/TSaw.hpp"
#include "pedal/TSquare.hpp"
#include "pedal/TTriangle.hpp"
#include "pedal/TSaw.hpp"
#include "pedal/CTEnvelope.hpp"

float currentSample;
TTriangle triangle;
TSine sine;
TSquare square;
TSaw saw;

CTEnvelope sustainedEnvelope;
CTEnvelope percussiveEnvelope;
//========================Audio Callback
void callback(float* out, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {
    
    sine.setFrequency(pdlGetSlider(app, 9));//set frequecy by slider
    triangle.setFrequency(pdlGetSlider(app, 9));//set frequecy by slider
    square.setFrequency(pdlGetSlider(app, 9));//set frequecy by slider
    saw.setFrequency(pdlGetSlider(app, 9));//set frequecy by slider
    
    square.setDutyCycle(pdlGetSlider(app, 4));
    
    //envelopes are ADSR by default
    sustainedEnvelope.setAttack(pdlGetSlider(app, 5));
    sustainedEnvelope.setDecay(pdlGetSlider(app, 6));
    sustainedEnvelope.setSustain(pdlGetSlider(app, 7));
    sustainedEnvelope.setRelease(pdlGetSlider(app, 8));
    
    percussiveEnvelope.setMode(CTEnvelope::AR);//set to attack-release
    percussiveEnvelope.setAttack(pdlGetSlider(app, 5));
    percussiveEnvelope.setRelease(pdlGetSlider(app, 8));

    bool trigger = pdlGetToggle(app, 0);//trigger envelope with toggle
    //bool trigger = pdlGetTrigger(app, 0);
    sustainedEnvelope.setTrigger(trigger);//set trigger to up or down, on or off, etc
    
    trigger = pdlGetTrigger(app, 0);
    percussiveEnvelope.setTrigger(trigger);
    float mx, my; pdlGetCursorPos(app, &mx, &my);//obtain mouse x and y coordinates

    //audio loop
    for (unsigned i = 0; i < buffer; i += 1) {//for entire buffer of frames
      //generate the envelope samples
      float envelopeOne = sustainedEnvelope.generateSample();
      float envelopeTwo = percussiveEnvelope.generateSample();

      float resultOne = 0.0f;//create a float for envelope one
      //add together the result of all oscillators with slider values
      resultOne += sine.generateSample() * pdlGetSlider(app, 0);
      resultOne += saw.generateSample() * pdlGetSlider(app, 1); 
      resultOne += triangle.generateSample() * pdlGetSlider(app, 2);
      resultOne += square.generateSample() * pdlGetSlider(app, 3);
      resultOne *= envelopeOne;//scale by the current value of the envelope
      
      float resultTwo = 0.0f;
      resultTwo += sine.getSample() * pdlGetSlider(app, 0);
      resultTwo += saw.getSample() * pdlGetSlider(app, 1); 
      resultTwo += triangle.getSample() * pdlGetSlider(app, 2);
      resultTwo += square.getSample() * pdlGetSlider(app, 3);
      resultTwo *= envelopeTwo;

      for (unsigned j = 0; j < channel; j += 1) {//for every sample in frame
        out[channel * i + j] = resultOne + resultTwo;
      }
    }
}
//======================main loop
int main() {
    //make an app (a pointer to an app, actually)
    pdlExampleApp* app = pdlInitExampleApp(callback);
    if (!app) {//if app doesn't succesfully allocate
      return 1;//cancel program, return 1
    }
    pdlSettings::sampleRate = pdlExampleAppGetSamplingRate(app);
    pdlSettings::bufferSize = pdlExampleAppGetBufferSize(app);
    // Add your GUI elements here
    pdlAddSlider(app, 0, "Sine", 0.0f, 1.0f, 0.7f);
    pdlAddSlider(app, 1, "Saw", 0.0f, 1.0f, 0.7f);
    pdlAddSlider(app, 2, "Triangle", 0.0f, 1.0f, 0.7f);
    pdlAddSlider(app, 3, "Square", 0.0f, 1.0f, 0.7f);
    pdlAddSlider(app, 4, "DutyCycle", 0.0f, 1.0f, 0.5f);

    pdlAddSlider(app, 5, "Attack", 2.0f, 300.0f, 80.0f);
    pdlAddSlider(app, 6, "Decay", 2.0f, 200.0f, 30.0f);
    pdlAddSlider(app, 7, "Sustain", 0.00f, 1.0f, 0.7f);
    pdlAddSlider(app, 8, "Release", 5.0f, 1000.0f, 200.0f);
    pdlAddSlider(app, 9, "Frequency", 1.0, 2000.0f, 100.0f);


    pdlAddToggle(app, 0, "loud", false);
    pdlAddTrigger(app, 0, "trigger");
    
    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}