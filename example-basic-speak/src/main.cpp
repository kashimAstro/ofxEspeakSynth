#include "ofMain.h"
#include "ofxEspeakSynth.h"

class ofApp : public ofBaseApp
{
	public:
		ofxEspeakSynth spk;

		void setup()
		{
			ofxEspeakSynth::ESParam es;
			es.language="en";
			es.gender=1;
			es.age=85;
			spk.setup(es);

			spk.speak("Dario. Pluto. Paperino. Kashimastro. Openframeworks. Opengl. Synth. Voice.");
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
