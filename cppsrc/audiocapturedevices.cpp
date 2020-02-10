#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <DSound.h>
#include <vector>
#include <Objbase.h>
#include "strconv.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "DSound.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Ole32.lib")


using namespace std;



#ifndef _WIN32
typedef struct _GUID {
	uint32_t Data1;
	uint16_t Data2;
	uint16_t Data3;
	uint8_t Data4[8];
} GUID;
#endif

GUID StringToGuid(const std::string& str)
{
	GUID guid;
	sscanf(str.c_str(),
	       "{%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx}",
	       &guid.Data1, &guid.Data2, &guid.Data3,
	       &guid.Data4[0], &guid.Data4[1], &guid.Data4[2], &guid.Data4[3],
	       &guid.Data4[4], &guid.Data4[5], &guid.Data4[6], &guid.Data4[7] );

	return guid;
}

std::string GuidToString(GUID guid)
{
	char guid_cstr[39];
	snprintf(guid_cstr, sizeof(guid_cstr),
	         "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
	         guid.Data1, guid.Data2, guid.Data3,
	         guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
	         guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	return std::string(guid_cstr);
}

LONG GetDWORDRegKey(HKEY hKey, const std::wstring &strValueName, DWORD &nValue, DWORD nDefaultValue)
{
    nValue = nDefaultValue;
    DWORD dwBufferSize(sizeof(DWORD));
    DWORD nResult(0);
    LONG nError = ::RegQueryValueExW(hKey,
        strValueName.c_str(),
        0,
        NULL,
        reinterpret_cast<LPBYTE>(&nResult),
        &dwBufferSize);
    if (ERROR_SUCCESS == nError)
    {
        nValue = nResult;
    }
    return nError;
}


LONG GetBoolRegKey(HKEY hKey, const std::wstring &strValueName, bool &bValue, bool bDefaultValue)
{
    DWORD nDefValue((bDefaultValue) ? 1 : 0);
    DWORD nResult(nDefValue);
    LONG nError = GetDWORDRegKey(hKey, strValueName.c_str(), nResult, nDefValue);
    if (ERROR_SUCCESS == nError)
    {
        bValue = (nResult != 0) ? true : false;
    }
    return nError;
}


LONG GetStringRegKey(HKEY hKey, const std::wstring &strValueName, std::wstring &strValue, const std::wstring &strDefaultValue)
{
    strValue = strDefaultValue;
    WCHAR szBuffer[512];
    DWORD dwBufferSize = sizeof(szBuffer);
    ULONG nError;
    nError = RegQueryValueExW(hKey, strValueName.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
    if (ERROR_SUCCESS == nError)
    {
        strValue = szBuffer;
    }
    return nError;
}



BOOL listAudioCaptureDevicesFromRegistry(std::vector<std::wstring> &names) {
    // https://superuser.com/questions/922008/windows-command-line-method-to-know-the-name-of-my-audio-recording-devices
	names = {};
	HKEY hKey;
	wstring subkey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\MMDevices\\Audio\\Capture";
	LONG lRes = RegOpenKeyExW(HKEY_LOCAL_MACHINE, subkey.c_str(), 0, KEY_READ | KEY_WOW64_64KEY, &hKey);
	bool bExistsAndSuccess (lRes == ERROR_SUCCESS);
	bool bDoesNotExistsSpecifically (lRes == ERROR_FILE_NOT_FOUND);
	if (!bExistsAndSuccess)
		return FALSE;

    DWORD cSubKeys;        //Used to store the number of Subkeys
    DWORD maxSubkeyLen;    //Longest Subkey name length
    DWORD cValues;        //Used to store the number of Subkeys
    DWORD maxValueLen;    //Longest Subkey name length
    DWORD retCode;        //Return values of calls
    RegQueryInfoKey(hKey,            // key handle
                    NULL,            // buffer for class name
                    NULL,            // size of class string
                    NULL,            // reserved
                    &cSubKeys,        // number of subkeys
                    &maxSubkeyLen,    // longest subkey length
                    NULL,            // longest class string 
                    &cValues,        // number of values for this key 
                    &maxValueLen,    // longest value name 
                    NULL,            // longest value data 
                    NULL,            // security descriptor 
                    NULL);            // last write time

    char currentSubkey[MAX_PATH];

    for(unsigned int i=0;i < cSubKeys;i++){
   		DWORD currentSubLen=MAX_PATH;

        retCode=RegEnumKeyEx(hKey,    // Handle to an open/predefined key
	        i,                // Index of the subkey to retrieve.
	        currentSubkey,            // buffer to receives the name of the subkey
	        &currentSubLen,            // size of that buffer
	        NULL,                // Reserved
	        NULL,                // buffer for class string 
	        NULL,                // size of that buffer
	        NULL);                // last write time

        if(retCode==ERROR_SUCCESS) {
            wstring t = s2ws(currentSubkey);
            wstring memberSubKey = subkey + L"\\" + t + L"\\Properties";
			HKEY hSubKey;
            lRes = RegOpenKeyExW(HKEY_LOCAL_MACHINE, memberSubKey.c_str(), 0, KEY_READ | KEY_WOW64_64KEY, &hSubKey);
			bExistsAndSuccess = (lRes == ERROR_SUCCESS);
			if (bExistsAndSuccess) {
				wstring type, name;
	            GetStringRegKey(hSubKey, L"{a45c254e-df1c-4efd-8020-67d146a850e0},2", type, L"bad");
	            GetStringRegKey(hSubKey, L"{b3f8fa53-0004-438e-9003-51a46e139bfc},6", name, L"bad");
	            if (type!=L"MIDI" && type!=L"bad") {
					names.push_back(type + L" (" + name + L")");
				}
				RegCloseKey(hSubKey);	
			}

		}
    }


	RegCloseKey(hKey);	
	return TRUE;
}


BOOL CALLBACK DSEnumCallback(LPGUID guid, LPCSTR descr, LPCSTR modname, LPVOID ctx)
{
    vector<wstring> *names = (vector<wstring>*)ctx;
    names->push_back(s2ws(string(descr)));
    return TRUE;
}


BOOL listAudioCaptureDevicesFromDirectSound(std::vector<std::wstring> &names) {
	names = {};
    if (FAILED(DirectSoundCaptureEnumerate(&DSEnumCallback, &names)))
    	return FALSE;
    return TRUE;
}

BOOL listAudioCaptureDevicesFromWaveIn(std::vector<std::wstring> &names) {
	names = {};
	WAVEINCAPS wcaps;
  	for (unsigned int i=0; i<waveInGetNumDevs(); i++) {
    	waveInGetDevCaps(i, &wcaps, sizeof(wcaps));
	    // std::string s = wcaps.szPname;
	   	names.push_back(s2ws(string(wcaps.szPname)));
	}  
	return TRUE;
}




BOOL listAudioCaptureDevicesFromWaveInAndRegistry(std::vector<std::wstring> &names) {
	std::vector<wstring> regnames = {};
	listAudioCaptureDevicesFromRegistry(regnames);
	names = {};
	WAVEINCAPS wcaps;
  	for (unsigned int i=0; i<waveInGetNumDevs(); i++) {
    	waveInGetDevCaps(i, &wcaps, sizeof(wcaps));

	    // std::string s = wcaps.szPname;
	    wstring ws = s2ws(string(wcaps.szPname));
	    for (unsigned int i=0; i<regnames.size(); i++)
	    	if(regnames[i].find(ws) == 0) 
	    		ws = regnames[i];
	   	names.push_back(ws);
	}  
	return TRUE;
}
