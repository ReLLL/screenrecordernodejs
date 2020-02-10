# Screen & Microphone A/V Recorder NodeJS Addon

(for Windows only)

Written using VC++ and N-API  

Uses ffmpeg.exe internally (embedded in package)
Partially uses Windows waveIn...() API to get device names

## EXAMPLE USAGE
Check examplea.js, examplev.js and example.js to see the usage
node example.js

### Audio recording

```js
const microphoneAddon = require('./index.js');

console.log('Listing the devices:');
console.log(microphoneAddon.listAudioRecordingDevices());


console.log('Start AudioRecording...');
const fullPathName = 'c://test.mp3';  // use .m4a extension for aac codec
const inputDeviceIndex = 0; // Record from the first device  (0 is default audio input device)
const codec = 'libmp3lame';  // aac, ac3, flac, opus, libmp3lame, wavpack    https://www.ffmpeg.org/ffmpeg-codecs.html
microphoneAddon.startAudioRecording(fullPathName, inputDeviceIndex, codec);



console.log('Waiting for 5 sec to have something recorded...');
function stopfunc() {
	console.log('Stop AudioRecording...');
	microphoneAddon.stopAudioRecording();
}
setTimeout(stopfunc, 5000);  // stop AudioRecording after 5 seconds
```

### Screen video recording

```js
const avrecorderAddon = require('./index.js');

console.log('Start Video Recording...');
const fullPathName = 'c://testvideo.mp4'; 
const x = 0;
const y = 0;
const width = 0;  
const height = 0;
const framerate = 30;
const codec = 'libx264'; //  libx264  https://www.ffmpeg.org/ffmpeg-codecs.html
avrecorderAddon.startVideoRecording(fullPathName, x, y, width, height, framerate, codec);



console.log('Waiting for 5 sec to have something recorded...');
function stopfunc() {
	console.log('Stop VideoRecording...');
	avrecorderAddon.stopVideoRecording();
}
setTimeout(stopfunc, 5000);  // stop VideoRecording after 5 seconds
```


### Recording both screen and microphone audio
```js
const avrecorderAddon = require('./index.js');

console.log('Start A/V Recording...');
const fullAudioPathName = 'c://testaudio.mp3';  // use .m4a extension for aac codec
const inputAudioDeviceIndex = 0; // Record from the first device (0 is default audio input device)
const audioCodec = 'libmp3lame';  // aac, ac3, flac, opus, libmp3lame, wavpack    https://www.ffmpeg.org/ffmpeg-codecs.html
const fullVideoPathName = 'c://testvideo.mp4'; 
const x = 0;  // ifscreen region coordinates are set as 0, entire desktop is recorded
const y = 0;
const width = 0;   
const height = 0;  
const framerate = 30;
const videoCodec = 'libx264'; //  libx264, libx265, libxvid, mpeg2, vc2 https://www.ffmpeg.org/ffmpeg-codecs.html
// NOTE: vc2 might be the best codec for screen recording, requires low overhead
avrecorderAddon.startRecording(fullVideoPathName, fullAudioPathName, inputAudioDeviceIndex, x, y, width, height, framerate, videoCodec, audioCodec);


console.log('Waiting for 5 sec to have something recorded...');
function stopfunc() {
	console.log('Stop A/V Recording...');
	avrecorderAddon.stopRecording();
}
setTimeout(stopfunc, 5000);  // stop VideoRecording after 5 seconds


```

## Exported functions

```js
function listAudioRecordingDevices() 

function startAudioRecording(fullPathName, inputDeviceIndex, codec)

function stopAudioRecording()

function startVideoRecording(fullPathName, x, y, width, height, framerate, codec)

function stopVideoRecording() 

function startRecording(fullVideoPathName, fullAudioPathName, inputAudioDeviceIndex, x, y, width, height, framerate, videoCodec, AudioCodec)

function stopRecording()
```


## BUILD STEPS
- Install VS 2019  (Express should be fine   https://visualstudio.microsoft.com/vs/express/)
- install the latest node version   
- Run the following command to build (in project directory)
```sh
npm run build 
```
- If it fails, please run the following commands (in project directory)
```sh
npm install node-gyp --save-dev
npm install node-addon-api
```


### Notes:
The development platform I used:
- Node v12.15.0 
- Visual Studio 2019 v16.4.4
- Windows Server 2019 (Version 1809)