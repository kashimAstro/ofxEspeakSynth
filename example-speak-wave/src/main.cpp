#include "ofMain.h"
#include "ofxEspeakSynth.h"

class ofApp : public ofBaseApp
{
	public:
		ofxEspeakSynth spk;
		vector<int> txtpos,audiopos,sample;

		void setup()
		{
			ofxEspeakSynth::ESParam es;
			es.language = "it";
			es.variant  = 0;
			es.gender   = 2;
			es.age      = 85;

			spk.setup(es);
			ofAddListener(spk.OutputData,this, &ofApp::out);
		}

		void out(ofxEspeakSynth::Synth & events)
		{
			if(txtpos.size()>500) txtpos.clear();
			if(audiopos.size()>500) audiopos.clear();
			if(sample.size()>500) sample.clear();
			txtpos.push_back( events.text_position );
			audiopos.push_back( events.audio_position );
			sample.push_back( events.sample ); 
		}

		void draw()
		{
			ofBackground(0);
			ofPushStyle();
			ofPushMatrix();
			ofNoFill();
			ofTranslate(10, 20, 0);
			ofSetColor(225);
			ofDrawBitmapString("Sample Audio Speak", 4, 18);
			ofSetLineWidth(1);
			ofDrawRectangle(0, 0, 512, 100);

			ofSetColor(245, 58, 135);
			ofSetLineWidth(3);
				ofBeginShape();
				for (unsigned int i = 0; i < sample.size(); i++){
					ofVertex(i*2, 60 -sample[i]/500);
				}
				ofEndShape(false);
			ofPopMatrix();
			ofPopStyle();

			ofPushStyle();
			ofPushMatrix();
			ofNoFill();
			ofTranslate(10, 150, 0);
			ofSetColor(225);
			ofDrawBitmapString("Text position", 4, 18);
			ofSetLineWidth(1);
			ofDrawRectangle(0, 0, 512, 100);

			ofSetColor(245, 58, 135);
			ofSetLineWidth(3);
				ofBeginShape();
				for (unsigned int i = 0; i < txtpos.size(); i++){
					ofVertex(i*2, 60 -txtpos[i]);
				}
				ofEndShape(false);
			ofPopMatrix();
			ofPopStyle();

			ofPushStyle();
			ofPushMatrix();
			ofNoFill();
			ofTranslate(10, 280, 0);
			ofSetColor(225);
			ofDrawBitmapString("Audio position", 4, 18);
			ofSetLineWidth(1);
			ofDrawRectangle(0, 0, 512, 100);

			ofSetColor(245, 58, 135);
			ofSetLineWidth(3);
				ofBeginShape();
				for (unsigned int i = 0; i < audiopos.size(); i++){
					ofVertex(i*2, 60 -audiopos[i]/200);
				}
				ofEndShape(false);
			ofPopMatrix();
			ofPopStyle();

		}

		void keyPressed( int key )
		{
			char d = key - ' ';

			stringstream letter;
			string s;
			letter << d;
			letter >> s;
			ofLog()<<s;
			spk.speak(s);
		}

};

int main()
{
	ofSetupOpenGL(1024,768, OF_WINDOW);
	ofRunApp( new ofApp());
}
