#include "processor.h"

#include <math.h>
#include <stdlib.h>

typedef struct {
  float exponent;
} PowerData;

static void powerSetFloatParam(Processor *processor, const char *param, float value) {
  PowerData *data = processor->data;
  PROCESSOR_PARAM(data, exponent);
}

static void powerSetStringParam(Processor *processor, const char *name, const char *value) {
}

static size_t powerBufferSize(Processor *processor, size_t inputSize) {
  return inputSize;
}

static void powerProcess(Processor *processor, const Buffer *src, Buffer *dst) {
  PowerData *data = processor->data;
  float usefulExponent = data->exponent < 1.0f ? 1.0f : data->exponent;
  float exponent = roundf(usefulExponent);
  int shouldCopySign = (int)(exponent) % 2 == 0;
  for(size_t i = 0; i < src->length; i++) {
    const Sample *srcSample = &src->samples[i];
    Sample *dstSample = &dst->samples[i];

    dstSample->left = powf(srcSample->left, exponent);
    if(shouldCopySign) {
      dstSample->left = copysignf(dstSample->left, srcSample->left);
    }
    dstSample->right = powf(srcSample->right, exponent);
    if(shouldCopySign) {
      dstSample->right = copysignf(dstSample->right, srcSample->right);
    }
  }
}

static void powerDestroy(Processor *processor) {
  free(processor->data);
}

Processor mkPowerProcessor(const char *name) {
  Processor processor = {
    .data = calloc(1, sizeof(PowerData)),
    .setFloatParam = powerSetFloatParam,
    .setStringParam = powerSetStringParam,
    .setBufferParam = noopSetBufferParam,
    .bufferSize = powerBufferSize,
    .process = powerProcess,
    .destroy = powerDestroy
  };
  return processor;
}
