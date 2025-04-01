#include "processor.h"

#include <stdlib.h>

typedef struct {
  float duration;
} FadeOutData;

static void fadeOutSetFloatParam(Processor *processor, const char *name, float value) {
  FadeOutData *data = processor->data;
  PROCESSOR_PARAM(data, duration);
}

static void fadeOutSetStringParam(Processor *processor, const char *name, const char *value) {
}

static size_t fadeOutBufferSize(Processor *processor, size_t inputSize) {
  return inputSize;
}

static void fadeOutProcess(Processor *processor, const Buffer *src, Buffer *dst) {
  FadeOutData *data = processor->data;
  size_t durSamples = src->sampleRate * data->duration;
  if(durSamples > src->length) {
    durSamples = src->length;
  }
  if(durSamples > dst->length) {
    durSamples = dst->length;
  }
  size_t start = src->length - durSamples;
  for(size_t i = 0; i < start; ++i) {
    dst->samples[i] = src->samples[i];
  }
  for(size_t i = start; i < src->length; i++) {
    float t = (float)(src->length - i) / durSamples;
    dst->samples[i].left = src->samples[i].left * t;
    dst->samples[i].right = src->samples[i].right * t;
  }
}

static void fadeOutDestroy(Processor *processor) {
  free(processor->data);
}

Processor mkFadeOutProcessor(void) {
  Processor processor = {
    .data = calloc(1, sizeof(FadeOutData)),
    .setFloatParam = fadeOutSetFloatParam,
    .setStringParam = fadeOutSetStringParam,
    .bufferSize = fadeOutBufferSize,
    .process = fadeOutProcess,
    .destroy = fadeOutDestroy
  };
  return processor;
}
