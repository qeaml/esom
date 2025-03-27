#pragma once

#include "sample.h"

#include <string.h>

typedef struct ProcessorS Processor;

typedef void(*ProcessorSetParamPfn)(Processor *processor, const char *param, float value);
typedef size_t(*ProcessorBufferSizePfn)(Processor *processor, size_t inputSize);
typedef void(*ProcessorProcessPfn)(Processor *processor, const Buffer *src, Buffer *dst);
typedef void(*ProcessorDestroyPfn)(Processor *processor);

struct ProcessorS {
  void *data;
  ProcessorSetParamPfn setParam;
  ProcessorBufferSizePfn bufferSize;
  ProcessorProcessPfn process;
  ProcessorDestroyPfn destroy;
};

void processorSetParam(Processor *processor, const char *param, float value);
size_t bufferSize(Processor *processor, size_t inputSize);
void processorProcess(Processor *processor, const Buffer *src, Buffer *dst);
void processorDestroy(Processor *processor);

Processor mkProcessor(const char *name);

Processor mkHardClipProcessor(void);
Processor mkGainProcessor(void);
Processor mkGlitchStretchProcessor(void);
Processor mkFadeInProcessor(void);
Processor mkFadeOutProcessor(void);

/* utility macro for processorSetParam implementations */
#define PROCESSOR_PARAM(data, paramName) \
  if(strcmp(name, #paramName) == 0) {    \
    data->paramName = value;             \
    return;                              \
  }

#define PROCESSOR_PARAM_BOOL(data, paramName) \
  if(strcmp(name, #paramName) == 0) {         \
    data->paramName = value != 0.0f;          \
    return;                                   \
  }
