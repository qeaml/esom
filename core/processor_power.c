#include "processor.h"

#include <math.h>
#include <stdlib.h>

typedef struct {
  float exponent;
} PowerData;

static void powerSetParam(Processor *processor, const char *name, float value) {
  PowerData *data = processor->data;
  PROCESSOR_PARAM(data, exponent);
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
  Processor processor;
  processor.data = malloc(sizeof(PowerData));
  PowerData *data = processor.data;
  data->exponent = 1.0f;
  processor.setParam = powerSetParam;
  processor.bufferSize = powerBufferSize;
  processor.process = powerProcess;
  processor.destroy = powerDestroy;
  return processor;
}
