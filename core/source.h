#pragma once

#include "sample.h"

#include <string.h>

typedef struct SourceS Source;

typedef void(*SourceSetFloatParamPfn)(Source *source, const char *param, float value);
typedef void(*SourceSetStringParamPfn)(Source *source, const char *param, const char *value);
typedef size_t(*SourceBufferSizePfn)(Source *source, int sampleRate);
typedef void(*SourceFillBufferPfn)(Source *source, Buffer *buffer);
typedef void(*SourceDestroyPfn)(Source *source);

struct SourceS {
  void *data;
  SourceSetFloatParamPfn setFloatParam;
  SourceSetStringParamPfn setStringParam;
  SourceBufferSizePfn bufferSize;
  SourceFillBufferPfn fillBuffer;
  SourceDestroyPfn destroy;
};

void sourceSetFloatParam(Source *source, const char *param, float value);
void sourceSetStringParam(Source *source, const char *param, const char *value);
#define sourceSetParam(source, param, value) \
  _Generic((value), \
    default: sourceSetFloatParam, \
    char*: sourceSetStringParam, \
    const char*: sourceSetStringParam \
  )(source, param, value)
size_t sourceBufferSize(Source *source, int sampleRate);
void sourceFillBuffer(Source *source, Buffer *buffer);
void sourceDestroy(Source *source);

Source mkSource(const char *name);

/* utility macro for sourceSetParam implementations */
#define SOURCE_PARAM(data, name)  \
  if(strcmp(param, #name) == 0) { \
    data->name = value;           \
    return;                       \
  }

#define SOURCE_PARAM_BOOL(data, name) \
  if(strcmp(param, #name) == 0) {     \
    data->name = value != 0.0f;       \
    return;                           \
  }

#define SOURCES(X) \
  X(Sine)          \
  X(Square)        \
  X(Noise)         \
  X(Sample)
