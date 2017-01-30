#include <node.h>
#include <nan.h>
#include <unistd.h>
#include "ad-da.h"

using namespace v8;

extern int initialized;

int _max_retries = 3;

class ReadWorker : public Nan::AsyncWorker {
  public:
    ReadWorker(Nan::Callback *callback, int sensor_type, int gpio_pin)
      : Nan::AsyncWorker(callback), sensor_type(sensor_type),
        gpio_pin(gpio_pin) { }

    void Execute() {
    }

    void HandleOKCallback() {
    }

  private:

    void Init() {
    }

    void Read() {
    }
};

void ReadAsync(const Nan::FunctionCallbackInfo<Value>& args) {
}

void ReadSync(const Nan::FunctionCallbackInfo<Value>& args) { 
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
