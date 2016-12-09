#include "ofxEspeakSynth.h"

ofxEspeakSynth::ofxEspeakSynth(){}
ofxEspeakSynth::~ofxEspeakSynth(){}
ofEvent<ofxEspeakSynth::Synth> ofxEspeakSynth::OutputData;

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

		ofNotifyEvent(ofxEspeakSynth::OutputData, setting);

		return 0;
}

void ofxEspeakSynth::exit()
{
	espeak_Terminate();
}

void ofxEspeakSynth::setup(ESParam _es)
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
