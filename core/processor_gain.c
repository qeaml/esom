#include "processor.h"

#include <stdlib.h>

typedef struct GainDataS {
  float gain;
} GainData;

static void gainSetParam(Processor *processor, const char *name, float value) {
  GainData *data = (GainData *)processor->data;
  PROCESSOR_PARAM(data, gain);
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
  Processor processor;
  processor.setParam = gainSetParam;
  processor.bufferSize = gainBufferSize;
  processor.process = gainProcess;
  processor.destroy = gainDestroy;
  processor.data = malloc(sizeof(GainData));
  GainData *data = (GainData *)processor.data;
  data->gain = 1.0;
  return processor;
}
