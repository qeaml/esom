#include "processor.h"

#include <stdlib.h>

typedef struct GainDataS {
  float gain;
} GainData;

static void gainSetFloatParam(Processor *processor, const char *name, float value) {
  GainData *data = (GainData *)processor->data;
  PROCESSOR_PARAM(data, gain);
}

static void gainSetStringParam(Processor *processor, const char *name, const char *value) {
}

static size_t gainBufferSize(Processor *processor, size_t inputSize) {
  return inputSize;
}

static void gainProcess(Processor *processor, const Buffer *src, Buffer *dst) {
  GainData *data = (GainData *)processor->data;
  for(size_t i = 0; i < dst->length; i++) {
    dst->samples[i].left = src->samples[i].left * data->gain;
    dst->samples[i].right = src->samples[i].right * data->gain;
  }
}

static void gainDestroy(Processor *processor) {
  free(processor->data);
}

Processor mkGainProcessor(void) {
  Processor processor = {
    .data = calloc(1, sizeof(GainData)),
    .setFloatParam = gainSetFloatParam,
    .setStringParam = gainSetStringParam,
    .bufferSize = gainBufferSize,
    .process = gainProcess,
    .destroy = gainDestroy
  };
  return processor;
}
