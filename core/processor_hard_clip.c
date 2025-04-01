#include "processor.h"

#include <math.h>
#include <stdlib.h>

typedef struct HardClipDataS {
  float threshold;
  int synched;
} HardClipData;

static void hardClipSetFloatParam(Processor *processor, const char *name, float value) {
  HardClipData *data = (HardClipData *)processor->data;
  PROCESSOR_PARAM(data, threshold);
  PROCESSOR_PARAM_BOOL(data, synched);
}

static void hardClipSetStringParam(Processor *processor, const char *name, const char *value) {
}

static size_t hardClipBufferSize(Processor *processor, size_t inputSize) {
  return inputSize;
}

static Sample clipSynched(Sample *sample, float thresh) {
  Sample out;
  if(fabs(sample->left) > thresh || fabs(sample->right) >= thresh) {
    out.left = copysignf(thresh, sample->left);
    out.right = copysignf(thresh, sample->right);
  } else {
    out = *sample;
  }
  return out;
}

static Sample clipIndep(Sample *sample, float thresh) {
  Sample out;
  if(fabs(sample->left) > thresh) {
    out.left = copysignf(thresh, sample->left);
  } else {
    out.left = sample->left;
  }
  if(fabs(sample->right) > thresh) {
    out.right = copysignf(thresh, sample->right);
  } else {
    out.right = sample->right;
  }
  return out;
}

static void hardClipProcess(Processor *processor, const Buffer *src, Buffer *dst) {
  HardClipData *data = (HardClipData *)processor->data;
  for(size_t i = 0; i < dst->length; i++) {
    const Sample *srcSample = &src->samples[i];
    Sample *dstSample = &dst->samples[i];
    if(data->synched) {
      dst->samples[i] = clipSynched(&src->samples[i], data->threshold);
    } else {
      dst->samples[i] = clipIndep(&src->samples[i], data->threshold);
    }
  }
}

static void hardClipDestroy(Processor *processor) {
  free(processor->data);
}

Processor mkHardClipProcessor(void) {
  Processor processor = {
    .data = calloc(1, sizeof(HardClipData)),
    .setFloatParam = hardClipSetFloatParam,
    .setStringParam = hardClipSetStringParam,
    .bufferSize = hardClipBufferSize,
    .process = hardClipProcess,
    .destroy = hardClipDestroy
  };
  return processor;
}
