#include <node.h>
#include <nan.h>
#include <unistd.h>
#include <mutex>
#include "ad-da.h"

using namespace v8;

extern int initialized;

std::mutex sensorMutex;

int _max_retries = 3;

class ReadWorker : public Nan::AsyncWorker {
  public:
    ReadWorker(Nan::Callback *callback)
      : Nan::AsyncWorker(callback) { }

    void Execute() {
      sensorMutex.lock();
      Init();
      Read();
      sensorMutex.unlock();
    }

    void HandleOKCallback() {
    }

  private:

    bool failed = false;

    void Init() {
      if (!initialized) {
        initialized = initialize() == 0;
      }
    }

    void Read() {
      int result = 0;
      int retry = _max_retries;
      while (true) {
        result = readADC();
        if (result == 0 || --retry < 0) break;
        usleep(450000);
      }
      failed = result != 0;
    }
};

void ReadAsync(const Nan::FunctionCallbackInfo<Value>& args) {
  Nan::Callback *callback = new Nan::Callback(args[2].As<Function>());

  Nan::AsyncQueueWorker(new ReadWorker(callback));
}

void ReadSync(const Nan::FunctionCallbackInfo<Value>& args) { 
  while (true) {
    int result = 0;
    int retry = _max_retries;
    result = readADC();
    if (result == 0 || --retry < 0) break;
    usleep(450000);
  }


  /*
  Local<Object> readout = Nan::New<Object>();
  readout->Set(Nan::New("humidity").ToLocalChecked(), Nan::New<Number>(humidity));
  readout->Set(Nan::New("temperature").ToLocalChecked(), Nan::New<Number>(temperature));
  readout->Set(Nan::New("isValid").ToLocalChecked(), Nan::New<Boolean>(result == 0));
  readout->Set(Nan::New("errors").ToLocalChecked(), Nan::New<Number>(_max_retries - retry));

  args.GetReturnValue().Set(readout);
  */
}


void Read(const Nan::FunctionCallbackInfo<Value>& args) {
	ReadSync(args);
}

void SetMaxRetries(const Nan::FunctionCallbackInfo<Value>& args) {
}

void Initialize(const Nan::FunctionCallbackInfo<Value>& args) {
}

void Init(Handle<Object> exports) {
	Nan::SetMethod(exports, "read", Read);
	Nan::SetMethod(exports, "initialize", Initialize);
  Nan::SetMethod(exports, "setMaxRetries", SetMaxRetries);
}

NODE_MODULE(node_ad_da, Init);
