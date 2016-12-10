#include "ofMain.h"

class Utility
{
	public:
	FILE *f_events = NULL;
        FILE *f_wavfile = NULL;
	unsigned int samples_total = 0;
	unsigned int previous_position=0;
	int samplerate;

	Utility()
	{
	}

	void opened(string _p)
	{
		f_events = fopen(_p.c_str(), "w");
		fprintf(f_events,"#Audacity format label format: <start_in_sec>\t<end_in_sec>\t<segment_labels>\n");
		fclose(f_events);
	}

	int waveRegister(string phonefile, string wavefile, short* wav, int numsamples, espeak_EVENT *events)
	{
		int type;
		if(wav == NULL)
		{
			closeWav();
			ofLog()<<"Close File";
			return 0;
		}
		f_events = fopen(phonefile.c_str(),"a");
		while((type = events->type) != 0)
		{
			if(events->type == espeakEVENT_SAMPLERATE)
			{
				samplerate = events->id.number;
			}
			else if(type==espeakEVENT_PHONEME)
			{
				const char *word = WordToString(events->id.number);
				fprintf(f_events,"%4.3f\t%4.3f\t%s\n", (float)previous_position/1000, (float)events->audio_position/1000,word );
				previous_position=events->audio_position;
			}
			events++;
		}
		fclose(f_events);

		if(f_wavfile == NULL)
		{
			if(openWav(wavefile, samplerate) != 0)
			{
				ofLog()<<"Open File";
				return 1;
			}
		}
		if(numsamples > 0)
		{
			samples_total += numsamples;
			fwrite(wav,numsamples*2,1,f_wavfile);
		}
		return 0;
	}

	void closeWav()
	{
		unsigned int pos;
		if((f_wavfile==NULL) || (f_wavfile == stdout))
			return;

		fflush(f_wavfile);
		pos = ftell(f_wavfile);
		fseek(f_wavfile,4,SEEK_SET);
		Write4Bytes(f_wavfile,pos - 8);
		fseek(f_wavfile,40,SEEK_SET);
		Write4Bytes(f_wavfile,pos - 44);
		fclose(f_wavfile);
		f_wavfile = NULL;
	}

	int openWav(string path, int rate)
	{
		static unsigned char wave_hdr[44] = {
			'R','I','F','F',0x24,0xf0,0xff,0x7f,'W','A','V','E','f','m','t',' ',
			0x10,0,0,0,1,0,1,0,  9,0x3d,0,0,0x12,0x7a,0,0,
			2,0,0x10,0,'d','a','t','a',  0x00,0xf0,0xff,0x7f};

		if(strcmp(path.c_str(),"stdout")==0)
			f_wavfile = stdout;
		else
			f_wavfile = fopen(path.c_str(),"wb");

		if(f_wavfile == NULL)
		{
			printf("Can't write to: '%s'\n",path.c_str());
			return 1;
		}
		fwrite(wave_hdr,1,24,f_wavfile);
		Write4Bytes(f_wavfile,rate);
		Write4Bytes(f_wavfile,rate * 2);
		fwrite(&wave_hdr[32],1,12,f_wavfile);
		return 0 ;
	}

	const char *WordToString(unsigned int word)
	{
		int ix;
		static char buf[5];

		for(ix=0; ix<3; ix++){
			buf[ix] = word >> (ix*8);
		}
		buf[4] = 0;
		return(buf);
	}

	void Write4Bytes(FILE *f, int value)
	{
		int ix;
		for(ix=0; ix<4; ix++)
		{
			fputc(value & 0xff,f);
			value = value >> 8;
		}
	}
};
