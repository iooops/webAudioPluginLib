// Web Audio Modules (WAMs)
// jari kleimola 2015-2017 (jari@webaudiomodules.org)
// WAM::Processor

#ifndef __WAM_processor__
#define __WAM_processor__

#include <emscripten.h>
#include <stdint.h>

namespace WAM {
		
typedef unsigned char byte;
typedef struct
{
	float** inputs;
	float** outputs;
} AudioBus;


class Processor
{
// -- lifecycle
public:
	Processor() {}
	virtual const char* init(uint32_t bufsize, uint32_t sr, void* desc);
	virtual void terminate() {}
	virtual void resize(uint32_t bufsize) {}
	virtual ~Processor() {}

// -- audio and data streams
public:
	virtual void onProcess(AudioBus* audio, void* data) = 0;
	virtual void onMidi(byte status, byte data1, byte data2) {}
	virtual void onSysex(byte* msg, uint32_t size) {}
	virtual void onMessage(char* verb, char* res, double data) {}
	virtual void onMessage(char* verb, char* res, char* data) {}
	virtual void onParam(uint32_t idparam, double value) {}	// todo: other datatypes
    uint32_t getUiDescSize();
    char* getUiDesc();
	
// -- patches
public:
	virtual void onPatch(void* data, uint32_t size) {}
	
// -- controller interface
protected:
	void postMessage(const char* verb, const char* resource, void* data) {}
	
protected:
	uint32_t m_bufsize;
	uint32_t m_sr;
	int m_inChannels;
	int m_outChannels;
    char* uiInfo_;
    uint32_t uiInfoSize_;
};

// for debugging
extern "C"
{
	void wam_logs(const char* s);
	void wam_logi(int i);
}

} // namespace WAM
#endif
