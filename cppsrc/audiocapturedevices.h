#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <DSound.h>
#include <vector>
#include <Objbase.h>


BOOL listAudioCaptureDevicesFromWaveIn(std::vector<std::wstring> &names);
BOOL listAudioCaptureDevicesFromDirectSound(std::vector<std::wstring> &names);
BOOL listAudioCaptureDevicesFromRegistry(std::vector<std::wstring> &names) ;
BOOL listAudioCaptureDevicesFromWaveInAndRegistry(std::vector<std::wstring> &names);