const avrecorderAddon = require('./index.js');

console.log('Listing the devices:');
console.log(avrecorderAddon.listAudioRecordingDevices());


console.log('Start Audio Recording...');
const fullPathName = 'c://testaudio.mp3';  // use .m4a extension for aac codec
const inputDeviceIndex = 0; // Record from the first device (0 = windows default audio input device (the first device in the list), -1 = ffmpeg default audio input device) 
const codec = 'libmp3lame';  // aac, ac3, flac, opus, libmp3lame, wavpack    https://www.ffmpeg.org/ffmpeg-codecs.html
avrecorderAddon.startAudioRecording(fullPathName, inputDeviceIndex, codec);



console.log('Waiting for 5 sec to have something recorded...');
function stopfunc() {
	console.log('Stop AudioRecording...');
	avrecorderAddon.stopAudioRecording();
}
setTimeout(stopfunc, 5000);  // stop AudioRecording after 5 seconds