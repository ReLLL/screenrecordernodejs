#include <windows.h>
#include "AudioRecorder.h"
#include <iostream>
#include <vector>

const int BUFFER_DURATION = 1;
const int CHANNEL_COUNT = 1;
const int SAMPLE_RATE = 44100;
const int DATA_SIZE = SAMPLE_RATE * BUFFER_DURATION * CHANNEL_COUNT;
const int BUFFER_SIZE = 10;
const int NUMBER_OF_BUFFERS = 5;


SoundBuffer::SoundBuffer()
{

}

void SoundBuffer::Init(int onumber_of_buffers) {
	this->number_of_buffers = onumber_of_buffers;
	ZeroMemory(&this->awaiting, sizeof(this->awaiting));
}

void SoundBuffer::SetAwaitingCompletion(int idx) {
	this->awaiting[idx] = 1;
}

void SoundBuffer::SetUnprepared(int idx) {
	this->awaiting[idx] = 0;
}

std::vector<int>  SoundBuffer::GetUnpreparedIDs() {
	std::vector<int> v = {};
	for (int i=0; i<1000; i++) 
		if (this->awaiting[i])
			v.push_back(i);
	return v;
}

SoundBuffer::~SoundBuffer()
{
}

SoundBuffer sb;
WAVEHDR* hdr1;
AudioRecorder::AudioRecorder()
{
	this->running = true;
	buffers[NUMBER_OF_BUFFERS] = {};
	
	sb.Init(NUMBER_OF_BUFFERS);
}


bool AudioRecorder::Init(void(*callback)(WAVEHDR data))
{
	this->callback = callback;

	ZeroMemory(&format, sizeof(format));
	format.nChannels = CHANNEL_COUNT;
	format.cbSize = 0;
	format.nSamplesPerSec = SAMPLE_RATE;
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.wBitsPerSample = 8;
	format.nBlockAlign = (CHANNEL_COUNT * format.wBitsPerSample) / 8;
	format.nAvgBytesPerSec = SAMPLE_RATE * format.nBlockAlign;
	
	res = waveInOpen(&micHandle, WAVE_MAPPER, &format, (DWORD_PTR)waveInCallback, 0, CALLBACK_FUNCTION );
	if (res != MMSYSERR_NOERROR)
	{
		std::cerr << "waveInOpen() failed" << std::endl;
		return false;
	}

	for (int i = 0; i < NUMBER_OF_BUFFERS; i++)
	{
		WAVEHDR header;
		ZeroMemory(&header, sizeof(header));
		

		header.lpData = (LPSTR)new short int[DATA_SIZE];
		header.dwBufferLength = DATA_SIZE;
		header.dwBytesRecorded = 0;
		header.dwUser = 0;
		header.dwFlags = 0;
		header.dwLoops = 0;
		header.dwUser = i;	

		buffers[i] = header;
		sb.SetAwaitingCompletion(i); 
		addBuffer(&buffers[i]);
	}
	
	return true;
}

bool AudioRecorder::addBuffer(WAVEHDR* buffer)
{
	res = waveInPrepareHeader(micHandle, buffer, sizeof(WAVEHDR));
	if (res != MMSYSERR_NOERROR)
	{
		std::cerr << "waveInPrepareHeader() failed" << std::endl;
		return false;
	}

	res = waveInAddBuffer(micHandle, buffer, sizeof(WAVEHDR));
	if (res != MMSYSERR_NOERROR)
	{
		std::cerr << "waveInAddBuffer() failed" << std::endl;
		return false;
	}
	return true;
}

void AudioRecorder::Update()
{
	std::vector<int> ids = sb.GetUnpreparedIDs();
	
	for each (int id in ids)
	{
		callback(buffers[id]);

		buffers[id].dwUser = id;
		addBuffer(&buffers[id]);

		sb.SetAwaitingCompletion(id);
	}

}

WAVEHDR AudioRecorder::getNewWAVEHDR()
{
	short int data[DATA_SIZE];
	WAVEHDR header;
	ZeroMemory(&header, sizeof(header));

	header.lpData = (LPSTR)&data;
	header.dwBufferLength = DATA_SIZE;
	header.dwBytesRecorded = 0;
	header.dwUser = 0;
	header.dwFlags = 0;
	header.dwLoops = 0;

	return header;
}

void CALLBACK waveInCallback(HWAVEIN hWaveOut, UINT uMesg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	if (uMesg == WIM_DATA)
	{
		WAVEHDR* hdr = (WAVEHDR*)dwParam1;
		int id = hdr->dwUser;

		sb.SetUnprepared(id);
	}
}

bool AudioRecorder::StartRecording()
{
	res = waveInStart(micHandle);
	if (res != MMSYSERR_NOERROR)
	{
		std::cerr << "waveInStart() failed" << std::endl;
		return false;
	}

	return true;
}

void AudioRecorder::StopRecording()
{
	waveInStop(micHandle);
}

AudioRecorder::~AudioRecorder()
{
}

