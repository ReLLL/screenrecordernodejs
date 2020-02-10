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