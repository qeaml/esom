#include "processor.h"

#include <stdlib.h>

typedef struct {
  float duration;
} FadeOutData;

static void fadeInSetParam(Processor *processor, const char *name, float value) {
  FadeOutData *data = processor->data;
  PROCESSOR_PARAM(data, duration);
}

static size_t fadeInBufferSize(Processor *processor, size_t inputSize) {
  return inputSize;
}

static void fadeInProcess(Processor *processor, const Buffer *src, Buffer *dst) {
  FadeOutData *data = processor->data;
  size_t durSamples = src->sampleRate * data->duration;
  for(size_t i = 0; i < durSamples; i++) {
    float t = (float)i / durSamples;
    dst->samples[i].left = src->samples[i].left * t;
    dst->samples[i].right = src->samples[i].right * t;
  }
  for(size_t i = durSamples; i < src->length; i++) {
    dst->samples[i] = src->samples[i];
  }
}

static void fadeInDestroy(Processor *processor) {
  free(processor->data);
}

Processor mkFadeInProcessor(void) {
  Processor processor = {
    .data = calloc(1, sizeof(FadeOutData)),
    .setParam = fadeInSetParam,
    .bufferSize = fadeInBufferSize,
    .process = fadeInProcess,
    .destroy = fadeInDestroy
  };
  return processor;
}
