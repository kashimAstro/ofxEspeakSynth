#include "ofMain.h"
#include "ofxEspeakSynth.h"

class ofApp : public ofBaseApp
{
	public:
		ofxEspeakSynth spk;

		void setup()
		{
			ofxEspeakSynth::ESParam es;
			es.language = "en";
			es.variant  = 0;
			es.gender   = 2;
			es.age      = 85;

			spk.setup(es);
			ofAddListener(spk.OutputData, this, &ofApp::SynthData);
		}

		void SynthData(ofxEspeakSynth::Synth & v)
		{
			ofLog()<<"pippo";
		}

		void draw()
		{

		}

		void keyPressed( int key )
		{
			if(key == ' ')
				spk.speak("keyboard pressed!");
		}

};

int main()
{
	ofSetupOpenGL(1024,768, OF_WINDOW);
	ofRunApp( new ofApp());
}
