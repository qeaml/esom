#pragma once

#include "sample.h"

#include <string.h>

typedef struct SourceS Source;

typedef void(*SourceSetParamPfn)(Source *source, const char *param, float value);
typedef size_t(*SourceBufferSizePfn)(Source *source, int sampleRate);
typedef void(*SourceFillBufferPfn)(Source *source, Buffer *buffer);
typedef void(*SourceDestroyPfn)(Source *source);

struct SourceS {
  void *data;
  SourceSetParamPfn setParam;
  SourceBufferSizePfn bufferSize;
  SourceFillBufferPfn fillBuffer;
  SourceDestroyPfn destroy;
};

void sourceSetParam(Source *source, const char *param, float value);
size_t sourceBufferSize(Source *source, int sampleRate);
void sourceFillBuffer(Source *source, Buffer *buffer);
void sourceDestroy(Source *source);

Source mkSource(const char *name);

Source mkSourceSine(void);
Source mkSourceSquare(void);
Source mkSourceNoise(void);

/* utility macro for sourceSetParam implementations */
#define SOURCE_PARAM(data, paramName) \
  if(strcmp(name, #paramName) == 0) {    \
    data->paramName = value;             \
    return;                              \
  }

#define SOURCE_PARAM_BOOL(data, paramName) \
  if(strcmp(name, #paramName) == 0) {         \
    data->paramName = value != 0.0f;          \
    return;                                   \
  }
