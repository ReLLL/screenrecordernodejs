#include <napi.h>
namespace recordfunc {


  int startAudioRecording(std::string fullPathName, int inputDeviceIndex, int mode, std::string ffmpegPath, std::string codec);
  Napi::Number startAudioRecordingWrapped(const Napi::CallbackInfo& info);

  int stopAudioRecording();
  Napi::Number stopAudioRecordingWrapped(const Napi::CallbackInfo& info);

  std::vector<std::string> listAudioRecordingDevices();
  Napi::Value listAudioRecordingDevicesWrapped(const Napi::CallbackInfo& info);




  int startVideoRecording(std::string fullPathName, int x,  int y, int width, int height, int framerate, std::string ffmpegPath, std::string codec);
  Napi::Number startVideoRecordingWrapped(const Napi::CallbackInfo& info);

  int stopVideoRecording();
  Napi::Number stopVideoRecordingWrapped(const Napi::CallbackInfo& info);




  Napi::Object Init(Napi::Env env, Napi::Object exports);
}