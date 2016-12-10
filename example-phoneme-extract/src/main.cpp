#include "ofMain.h"
#include "ofxEspeakSynth.h"

class ofApp : public ofBaseApp
{
	public:
		ofxEspeakSynth spk;

		void setup()
		{
			ofxEspeakSynth::ESParam es;
			es.language = "mb-it1";
			es.gender   = 1;
			es.age      = 85;
			es.value    = 20;
			es.rate     = espeakWORDGAP;
			es.option   = espeakINITIALIZE_PHONEME_EVENTS;
			es.output   = AUDIO_OUTPUT_SYNCHRONOUS;
			es.buflen   = 0;
			spk.setup(es);
			spk.setRegisterPhoneWave("phoneme.txt","wave.au");
			spk.speak("Openframeworks, OpenGL, Synthesizer, Synth, Espeak, Voice");
		}

		void keyPressed( int key )
		{
			if(key == ' ') 
			{
				spk.setRegisterPhoneWave("phoneme1.txt","wave1.au");
				spk.speak("Keyboard Pressed, Keyboard Pressed, Keyboard Pressed, Keyboard Pressed, Keyboard Pressed, Keyboard Pressed");
			}

		}
};

int main()
{
	ofSetupOpenGL(1024,768, OF_WINDOW);
	ofRunApp( new ofApp());
}
