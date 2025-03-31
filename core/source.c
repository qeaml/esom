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

#define SOURCE_MK_FUNC(name) \
  Source mk##name##Source(void);

SOURCES(SOURCE_MK_FUNC)

#define SOURCE_MK(sourceName)          \
  if(strcmp(name, #sourceName) == 0) { \
    return mk##sourceName##Source();   \
  }

Source mkSource(const char *name) {
  SOURCES(SOURCE_MK)
  return (Source){NULL};
}
