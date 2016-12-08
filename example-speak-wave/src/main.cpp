#include "ofMain.h"
#include "ofxEspeakSynth.h"

class ofApp : public ofBaseApp
{
	public:
		ofxEspeakSynth spk;
                ofSoundStream soundStream;
		vector <float> left;
                vector <float> right;
                vector <float> volHistory;
                float smoothedVol;
                int bufferCounter;

		void audioIn(float * input, int bufferSize, int nChannels){
			float curVol = 0.0;
			int numCounted = 0;
			for (int i = 0; i < bufferSize; i++)
			{
				left[i]         = input[i*2]*0.5;
				right[i]        = input[i*2+1]*0.5;
				curVol += left[i] * left[i];
				curVol += right[i] * right[i];
				numCounted+=2;
			}
			curVol /= (float)numCounted;
			curVol = sqrt( curVol );
			smoothedVol *= 0.93;
			smoothedVol += 0.07 * curVol;
			bufferCounter++;
		}


		void setup()
		{
			ofxEspeakSynth::ESParam es;
			es.language = "it";
			es.variant  = 0;
			es.gender   = 2;
			es.age      = 85;

			spk.setup(es);
			int bufferSize = 256;
		        left.assign(bufferSize, 0.0);
		        right.assign(bufferSize, 0.0);
		        volHistory.assign(400, 0.0);
		        soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
		}

		void draw()
		{
			// draw the left channel:
			ofNoFill();
			ofPushStyle();
				ofPushMatrix();
				ofTranslate(32, 170, 0);

				ofSetColor(225);
				ofDrawBitmapString("Left Channel", 4, 18);

				ofSetLineWidth(1);
				ofDrawRectangle(0, 0, 512, 200);

				ofSetColor(245, 58, 135);
				ofSetLineWidth(3);

					ofBeginShape();
					for (unsigned int i = 0; i < left.size(); i++){
						ofVertex(i*2, 100 -left[i]*180.0f);
					}
					ofEndShape(false);

				ofPopMatrix();
			ofPopStyle();
			// draw the right channel:
			ofPushStyle();
				ofPushMatrix();
				ofTranslate(32, 370, 0);

				ofSetColor(225);
				ofDrawBitmapString("Right Channel", 4, 18);

				ofSetLineWidth(1);
				ofDrawRectangle(0, 0, 512, 200);

				ofSetColor(245, 58, 135);
				ofSetLineWidth(3);

					ofBeginShape();
					for (unsigned int i = 0; i < right.size(); i++){
						ofVertex(i*2, 100 -right[i]*180.0f);
					}
					ofEndShape(false);

				ofPopMatrix();
			ofPopStyle();
		}

		void keyPressed( int key )
		{
			if(key == ' ') spk.speak("Premuto Tasto Space!");
			if(key == 'q') spk.speak("Q");
			if(key == 'w') spk.speak("W");
			if(key == 'e') spk.speak("E");
			if(key == 'r') spk.speak("R");
			if(key == 't') spk.speak("T");
			if(key == 'y') spk.speak("Y");
		}

};

int main()
{
	ofSetupOpenGL(1024,768, OF_WINDOW);
	ofRunApp( new ofApp());
}
