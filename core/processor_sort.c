#include "processor.h"

#include <stdlib.h>

static size_t sortBufferSize(Processor *processor, size_t inputSize) {
  return inputSize;
}

static int cmpSamples(const void *ptrA, const void *ptrB) {
  const Sample *sampleA = ptrA;
  const Sample *sampleB = ptrB;
  if(sampleA->left < sampleB->left) {
    return -1;
  } else if(sampleA->left > sampleB->left) {
    return 1;
  } else {
    return 0;
  }
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static void sortProcess(Processor *processor, const Buffer *src, Buffer *dst) {
  size_t safeLen = MIN(src->length, dst->length);
  for(size_t i = 0; i < safeLen; ++i) {
    dst->samples[i] = src->samples[i];
  }
  qsort(dst->samples, safeLen, sizeof(Sample), &cmpSamples);
}

Processor mkSortProcessor() {
  Processor processor = {
    .data = NULL,
    .setFloatParam = noopSetFloatParam,
    .setStringParam = noopSetStringParam,
    .setBufferParam = noopSetBufferParam,
    .bufferSize = sortBufferSize,
    .process = sortProcess,
    .destroy = noopDestroy,
  };
  return processor;
}
