#include "ofxEspeakSynth.h"

ofxEspeakSynth::ofxEspeakSynth(){}
ofxEspeakSynth::~ofxEspeakSynth(){}

ofEvent<ofxEspeakSynth::Synth> ofxEspeakSynth::OutputData;
Utility ofxEspeakSynth::utility;
string ofxEspeakSynth::phonefile;
string ofxEspeakSynth::wavefile;
bool ofxEspeakSynth::xregister;

int ofxEspeakSynth::synthcall(short *wav, int numsamples, espeak_EVENT *events)
{
		Synth setting;
		setting.type               = events->type;
		setting.unique_identifier  = events->unique_identifier;
		setting.text_position      = events->text_position;
		setting.length             = events->length;
		setting.audio_position     = events->audio_position;
		setting.sample             = events->sample;
		setting.user_data          = events->user_data;
		setting.wav                = wav;
		setting.numsamples         = numsamples;

		ofNotifyEvent(ofxEspeakSynth::OutputData, setting);

		if( xregister )
		{
			utility.waveRegister(phonefile, wavefile, wav, numsamples, events);
		}
		return 0;
}

void ofxEspeakSynth::setRegisterPhoneWave(string _phonefile, string _wavefile)
{
	phonefile = ofToDataPath(_phonefile,true);
	wavefile  = ofToDataPath(_wavefile,true);
	utility.opened(phonefile);
	xregister = true;
}

void ofxEspeakSynth::exit()
{
	espeak_Terminate();
	xregister = false;
}

void ofxEspeakSynth::setup(ESParam _es)
{
	Buflength    = _es.buflen;
	Options      = _es.option;
	path         = NULL;
	position     = 0;
	end_position = 0;
	flags        = _es.flags;
	rate         = _es.rate;
	value        = _es.value;
	
	output = _es.output;

	espeak_Initialize( output, Buflength, path, Options );
	espeak_SetParameter(rate,value,0);

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

void ofxEspeakSynth::speak(string _text)
{
	size = _text.length()+1;
	text = _text;
	startThread(true);
}

void ofxEspeakSynth::threadedFunction() 
{
	while(isThreadRunning()) 
	{
		espeak_Synth( text.c_str(), size, position, position_type, end_position, flags, unique_identifier, user_data );
		espeak_Synchronize();
		stopThread();
	}
}
