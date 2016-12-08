/*
	kashimAstro Dario Longobardi
	Wrapper espeak library for openframeworks

	http://espeak.sourceforge.net/speak_lib.h


		name:      NULL, or a voice name

		languages: NULL, or a single language string
                           (with optional dialect), eg. "en-uk", or "en"

                gender:    0=not specified, 1=male, 2=female

                age:       0=not specified, or an age in years

                variant:   After a list of candidates is produced, 
                           scored and sorted, "variant" is used to index
                           that list and choose a voice.
                           variant=0 takes the top voice 
                           (i.e. best match). variant=1 takes the next voice, etc

		flags:     #define espeakCHARS_AUTO   0
			   #define espeakCHARS_UTF8   1
			   #define espeakCHARS_8BIT   2
			   #define espeakCHARS_WCHAR  3
			   #define espeakCHARS_16BIT  4
			
			   //??
			   #define espeakSSML          0x10
  			   #define espeakPHONEMES      0x100
			   #define espeakENDPAUSE      0x1000
			   #define espeakKEEP_NAMEDATA 0x2000
	
	         AUDIO_OUTPUT_PLAYBACK:
			   PLAYBACK mode: plays the audio data, supplies events to the calling program

		 AUDIO_OUTPUT_RETRIEVAL:
		           RETRIEVAL mode: supplies audio data and events to the calling program 

		 AUDIO_OUTPUT_SYNCHRONOUS:
		           SYNCHRONOUS mode: as RETRIEVAL but doesn't return until synthesis is completed 

		 AUDIO_OUTPUT_SYNCH_PLAYBACK:
		           Synchronous playback

*/

#include <espeak/speak_lib.h>
#include "ofMain.h"

class ofxEspeakSynth : public ofThread
{
	public:

	struct ESParam
	{
		string name                = "espeak-default-name";
		string language     	   = "en-uk";
		unsigned int flags  	   = espeakCHARS_AUTO;
		int variant         	   = 2;
		int gender          	   = 1;
		int age             	   = 0;
		int buflen                 = 500;
		espeak_AUDIO_OUTPUT output = AUDIO_OUTPUT_PLAYBACK;
	};

	struct Synth
	{
		  espeak_EVENT_TYPE type;
                  unsigned int unique_identifier  = 0;
                  int text_position               = 0;
                  int length                      = 0;
                  int audio_position              = 0;
                  int sample                      = 0;
                  void* user_data                 = 0;
	};

	ofEvent<Synth> OutputData;
	espeak_POSITION_TYPE position_type;
	espeak_AUDIO_OUTPUT output;
	espeak_PARAMETER Parm;

	int Buflength;
	int Options;
	char *path;
	void *user_data;
	string text;
	unsigned int size;
	unsigned int position;
        unsigned int end_position;
        unsigned int flags;
        unsigned int *unique_identifier;

	ofxEspeakSynth() { }
	~ofxEspeakSynth() { }

	static int synthcall(short *wav, int numsamples, espeak_EVENT *events)
        {
			Synth setting;
			setting.type               = events->type;
                  	setting.unique_identifier  = events->unique_identifier;
	                setting.text_position      = events->text_position;
			setting.length             = events->length;
                  	setting.audio_position     = events->audio_position;
	                setting.sample             = events->sample;
        	        setting.user_data          = events->user_data;

			ofLog()<<"Type::"<<events->type;
			ofLog()<<"Unique::"<<events->unique_identifier;
			ofLog()<<"Text-position::"<<events->text_position;
			ofLog()<<"Length::"<<events->length;
			ofLog()<<"Audio-position::"<<events->audio_position;
			ofLog()<<"Sample::"<<events->sample;
			ofLog()<<"User-data::"<<events->user_data;

			//ofNotifyEvent(labsInstance->touchEventDataEventDispatcher, event);

			return 0;
        }

	void exit()
	{
		espeak_Terminate();
	}

	void setup(ESParam _es)
	{
		Buflength    = _es.buflen;
		Options      = 0;
		path         = NULL;
		position     = 0;
		end_position = 0;
		flags        = _es.flags;
		
		output = _es.output;

		espeak_Initialize( output, Buflength, path, Options );
                espeak_SetSynthCallback( synthcall );

		espeak_VOICE voice;
	        memset(&voice, 0, sizeof(espeak_VOICE));

	        voice.languages = _es.language.c_str();
	        voice.name      = _es.name.c_str();
	        voice.variant   = _es.variant;
	        voice.gender    = _es.gender;
	        voice.age       = _es.age;

	        espeak_SetVoiceByProperties(&voice);
	}
        
	void speak(string _text)
	{
		size = _text.length()+1;
		text = _text;
		startThread(true);
	}

        void threadedFunction() 
	{
        	while(isThreadRunning()) 
		{
			espeak_Synth( text.c_str(), size, position, position_type, end_position, flags, unique_identifier, user_data );
	                espeak_Synchronize();
			stopThread();
		}
	}
	/* End */
};
