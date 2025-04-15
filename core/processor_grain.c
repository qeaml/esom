#include "processor.h"
#include <stdlib.h>

typedef struct {
  float grainSize;
  float grainAdvance;
} GrainData;

static void grainSetFloatParam(Processor *processor, const char *param, float value) {
  GrainData *data = processor->data;
  PROCESSOR_PARAM(data, grainSize);
  PROCESSOR_PARAM(data, grainAdvance);
}

static size_t grainBufferSize(Processor *processor, size_t inputSize) {
  GrainData *data = processor->data;
  float factor = data->grainSize / data->grainAdvance;
  return inputSize * factor;
}

static void grainProcess(Processor *processor, const Buffer *src, Buffer *dst) {
  GrainData *data = processor->data;
  size_t grainSize = data->grainSize * src->sampleRate;
  if(grainSize == 0) {
    grainSize = 1;
  }
  size_t grainAdvance = data->grainAdvance * src->sampleRate;
  if(grainAdvance == 0) {
    grainAdvance = 1;
  }
  size_t srcOff = 0;
  size_t dstOff = 0;
  while(srcOff + grainSize < src->length) {
    for(size_t i = 0; i < grainSize; i++) {
      if(srcOff + i >= src->length || dstOff + i >= dst->length) {
        break;
      }
      dst->samples[dstOff + i] = src->samples[srcOff + i];
    }
    srcOff += grainAdvance;
    dstOff += grainSize;
  }
}

static void grainDestroy(Processor *processor) {
  free(processor->data);
}

Processor mkGrainProcessor() {
  Processor processor = {
    .data = calloc(1, sizeof(GrainData)),
    .setFloatParam = grainSetFloatParam,
    .setStringParam = noopSetStringParam,
    .setBufferParam = noopSetBufferParam,
    .bufferSize = grainBufferSize,
    .process = grainProcess,
    .destroy = grainDestroy
  };
  return processor;
}
