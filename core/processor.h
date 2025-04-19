#pragma once

#include "sample.h"

#include <string.h>

typedef struct ProcessorS Processor;

typedef void(*ProcessorSetFloatParamPfn)(Processor *processor, const char *param, float value);
typedef void(*ProcessorSetStringParamPfn)(Processor *processor, const char *param, const char *value);
typedef void(*ProcessorSetBufferParamPfn)(Processor *processor, const char *param, const Buffer *value);
typedef size_t(*ProcessorBufferSizePfn)(Processor *processor, size_t inputSize);
typedef void(*ProcessorProcessPfn)(Processor *processor, const Buffer *src, Buffer *dst);
typedef void(*ProcessorDestroyPfn)(Processor *processor);

struct ProcessorS {
  void *data;
  ProcessorSetFloatParamPfn setFloatParam;
  ProcessorSetStringParamPfn setStringParam;
  ProcessorSetBufferParamPfn setBufferParam;
  ProcessorBufferSizePfn bufferSize;
  ProcessorProcessPfn process;
  ProcessorDestroyPfn destroy;
};

void processorSetFloatParam(Processor *processor, const char *param, float value);
void processorSetStringParam(Processor *processor, const char *param, const char *value);
void processorSetBufferParam(Processor *processor, const char *param, const Buffer *value);
#define processorSetParam(processor, name, value) \
  _Generic((value),                               \
    default: processorSetFloatParam,              \
    char*: processorSetStringParam,               \
    const char*: processorSetStringParam,         \
    Buffer*: processorSetBufferParam,             \
    const Buffer*: processorSetBufferParam        \
  )(processor, name, value)
size_t processorBufferSize(Processor *processor, size_t inputSize);
void processorProcess(Processor *processor, const Buffer *src, Buffer *dst);
void processorDestroy(Processor *processor);

void noopSetFloatParam(Processor *processor, const char *param, float value);
void noopSetStringParam(Processor *processor, const char *param, const char *value);
void noopSetBufferParam(Processor *processor, const char *param, const Buffer *value);
void noopDestroy(Processor *processor);

Processor mkProcessor(const char *name);

/* utility macro for processorSetParam implementations */
#define PROCESSOR_PARAM(data, paramName) \
  if(strcmp(param, #paramName) == 0) {   \
    data->paramName = value;             \
    return;                              \
  }

#define PROCESSOR_PARAM_BOOL(data, paramName) \
  if(strcmp(param, #paramName) == 0) {        \
    data->paramName = value != 0.0f;          \
    return;                                   \
  }

#define PROCESSORS(X) \
  X(HardClip) \
  X(Gain) \
  X(GlitchStretch) \
  X(InterpStretch) \
  X(GrainStretch) \
  X(FadeIn) \
  X(FadeOut) \
  X(Average) \
  X(Power) \
  X(Formula) \
  X(Grain) \
  X(Sort)
