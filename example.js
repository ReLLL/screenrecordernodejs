const avrecorderAddon = require('./index.js');

console.log('Start A/V Recording...');
const fullAudioPathName = 'c://testaudio.mp3';  // use .m4a extension for aac codec
const inputAudioDeviceIndex = 0; // Record from the first device (0 = windows default audio input device (the first device in the list) -1 = ffmpeg default audio input device) 
const audioCodec = 'libmp3lame';  // aac, ac3, flac, opus, libmp3lame, wavpack    https://www.ffmpeg.org/ffmpeg-codecs.html
const fullVideoPathName = 'c://testvideo.mp4'; 
const x = 0;  // if screen region coordinates are set as 0, entire desktop is recorded
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
