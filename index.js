const avrecorderAddon = require('./build/Release/avrecorder.node');
var path = require('path');


module.exports.listAudioRecordingDevices = function listAudioRecordingDevices() {
	return avrecorderAddon.listAudioRecordingDevices();
}



module.exports.startAudioRecording = function startAudioRecording(fullPathName, inputDeviceIndex, codec) {
	// get relative path of ffmpeg.exe embedded inside package
	var ffmpegPath = path.join(__dirname, 'embed', 'ffmpeg.exe');  
	console.log('[VERBOSE] ' + ffmpegPath);

	const mode = 1;  // mode 0=WindowsAPI/WAV  1=ffmpeg/MP4   (please use mode 1 for now,  WAV headers are complicated and can take time to implement)
	return avrecorderAddon.startAudioRecording(fullPathName, inputDeviceIndex, mode, ffmpegPath, codec);
}

module.exports.stopAudioRecording = function stopAudioRecording() {
	return avrecorderAddon.stopAudioRecording();
}




module.exports.startVideoRecording = function startVideoRecording(fullPathName, x, y, width, height, framerate, codec) {
	// get relative path of ffmpeg.exe embedded inside package
	var ffmpegPath = path.join(__dirname, 'embed', 'ffmpeg.exe');  
	console.log('[VERBOSE] ' + ffmpegPath);
	return avrecorderAddon.startVideoRecording(fullPathName, x, y, width, height, framerate, ffmpegPath, codec);
}

module.exports.stopVideoRecording = function stopVideoRecording() {
	return avrecorderAddon.stopVideoRecording();
}




module.exports.startRecording = function startRecording(fullVideoPathName, fullAudioPathName, inputAudioDeviceIndex, x, y, width, height, framerate, videoCodec, AudioCodec) {
	// get relative path of ffmpeg.exe embedded inside package
	var ffmpegPath = path.join(__dirname, 'embed', 'ffmpeg.exe');  
	console.log('[VERBOSE] ' + ffmpegPath);

	const mode = 1;  // mode 0=WindowsAPI/WAV  1=ffmpeg/MP4   (please use mode 1 for now,  WAV headers are complicated and can take time to implement)
	var retA = avrecorderAddon.startAudioRecording(fullAudioPathName, inputAudioDeviceIndex, mode, ffmpegPath, AudioCodec);
	var retV = avrecorderAddon.startVideoRecording(fullVideoPathName, x, y, width, height, framerate, ffmpegPath, videoCodec);
	return retA + retV; // if one of them fails, returns any number other than 0
}

module.exports.stopRecording = function stopRecording() {
	var retA = avrecorderAddon.stopAudioRecording();
	var retV = avrecorderAddon.stopVideoRecording();
	return retA + retV; // if one of them fails, returns any number other than 0
}

