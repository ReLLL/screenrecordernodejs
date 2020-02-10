#pragma once
#include <windows.h>
#include <iostream>
#include <vector>

class SoundBuffer 
{
public:
	SoundBuffer();
	~SoundBuffer();

	void Init(int onumber_of_buffers);
	void SetAwaitingCompletion(int idx);
	void SetUnprepared(int idx);
	std::vector<int> GetUnpreparedIDs();

private:
	int number_of_buffers;
	int awaiting[1000];
};


#pragma warning( push )
#pragma warning( disable : 4200)
class AudioRecorder
{
public:
	AudioRecorder();
	~AudioRecorder();
	void Update();

	bool Init(void(*callback)(WAVEHDR data));

	bool StartRecording();
	void StopRecording();

	WAVEHDR getNewWAVEHDR();

private:
	void(*callback)(WAVEHDR data);
	bool running;  

	MMRESULT res;
	HWAVEIN micHandle;
	WAVEFORMATEX format;
	WAVEHDR buffers[];


	bool addBuffer(WAVEHDR *buffer);
};
#pragma warning( pop ) 

static void CALLBACK waveInCallback(HWAVEIN hWaveOut, UINT uMesg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
void playData(WAVEHDR hdr);
	
	 