#include "source.h"

void sourceSetParam(Source *source, const char *param, float value) {
  source->setParam(source, param, value);
}

size_t sourceBufferSize(Source *source, int sampleRate) {
  return source->bufferSize(source, sampleRate);
}

void sourceFillBuffer(Source *source, Buffer *buffer) {
  source->fillBuffer(source, buffer);
}

void sourceDestroy(Source *source) {
  source->destroy(source);
}

#define SOURCE(sourceName, factory) \
  if(strcmp(name, #sourceName) == 0) { \
    return factory();                  \
  }

Source mkSource(const char *name) {
  SOURCE(sine, mkSourceSine);
  SOURCE(square, mkSourceSquare);
  SOURCE(noise, mkSourceNoise);
  return (Source){NULL};
}
