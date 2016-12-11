# ofxEspeakSynth

<b>Origin:</b><br>
http://espeak.sourceforge.net/

<b>API:</b><br>
http://espeak.sourceforge.net/speak_lib.h

Text to Speech synthesizer for openframeworks basic espeak.
<br>
<pre><code>
ofxEspeakSynth spk;

ofxEspeakSynth::ESParam es;
es.language="en";
spk.setup(es);
spk.speak("Openframeworks, Synth, Voice.");
</code></pre>

<br>
<h1><b>Dependencies:</b></h1><br>
<h1>Linux</h1><br>
<pre><code>apt-get install libespeak-dev portaudio19-dev</code></pre>

<h1>OSX</h1><br>
http://sourceforge.net/projects/espeak/files/espeak/espeak-1.45/espeak-1.45.04-OSX.zip

<h1>Windows</h1><br>
http://sourceforge.net/projects/espeak/files/espeak/espeak-1.48/setup_espeak-1.48.04.exe
