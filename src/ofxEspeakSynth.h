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
#include "Utility.h"

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
		int option                 = 0;
		int value                  = 20;
		espeak_PARAMETER rate      = espeakWORDGAP;
		espeak_AUDIO_OUTPUT output = AUDIO_OUTPUT_PLAYBACK;
		/*
		   parameter:
		      espeakRATE:    speaking speed in word per minute.  Values 80 to 450.

		      espeakVOLUME:  volume in range 0-200 or more.
				     0=silence, 100=normal full volume, greater 
				     values may produce amplitude compression or distortion

		      espeakPITCH:   base pitch, range 0-100.  50=normal

		      espeakRANGE:   pitch range, range 0-100. 0-monotone, 50=normal

		      espeakPUNCTUATION: which punctuation characters to announce:
			                 value in espeak_PUNCT_TYPE (none, all, some),
                                         see espeak_GetParameter() to specify
                                         which characters are announced.

		      espeakCAPITALS: announce capital letters by:
			 0=none,
			 1=sound icon,
			 2=spelling,
			 3 or higher, by raising pitch.  This values
                           gives the amount in Hz by which the pitch
			   of a word raised to indicate it has a capital letter.

		      espeakWORDGAP:  pause between words, units of 10mS (at the default speed)*/
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
		  short* wav                      = NULL;
		  int numsamples                  = 0;  
	};
	static Utility utility;
	static ofEvent<Synth> OutputData;
	espeak_POSITION_TYPE position_type;
	espeak_AUDIO_OUTPUT output;
	espeak_PARAMETER Parm;

	int Buflength;
	int Options;
	char *path;
	void *user_data;
	string text;
        static string phonefile;
        static string wavefile;
        static bool xregister;
	unsigned int size;
	unsigned int position;
        unsigned int end_position;
        unsigned int flags;
        unsigned int *unique_identifier;
	espeak_PARAMETER rate;
	int value;

	ofxEspeakSynth();
	~ofxEspeakSynth();
	void exit();
	void setup(ESParam _es);
	void speak(string _text);
	void setRegisterPhoneWave(string _phonefile, string _wavefile);

	//
	private:
	static int synthcall(short *wav, int numsamples, espeak_EVENT *events);
        void threadedFunction();
	/* End */
};
