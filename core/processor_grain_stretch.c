#include "processor.h"

#include <stdlib.h>

typedef struct {
  float factor;
  float granularity;
} GrainStretchData;

static void grainStretchSetParam(Processor *processor, const char *name, float value) {
  GrainStretchData *data = processor->data;

  PROCESSOR_PARAM(data, factor)
  PROCESSOR_PARAM(data, granularity)
}

static size_t grainStretchBufferSize(Processor *processor, size_t inputSize) {
  GrainStretchData *data = processor->data;
  return inputSize * data->factor;
}

static void grainStretchProcess(Processor *processor, const Buffer *src, Buffer *dst) {
  GrainStretchData *data = processor->data;

  size_t chunkCount = src->length / data->factor;
  size_t chunkSize = src->length / chunkCount;
  size_t grainCount = chunkSize / data->granularity;
  size_t grainSize = chunkSize / grainCount;
  for(size_t chunkNo = 0; chunkNo < chunkCount; ++chunkNo) {
    for(size_t grainNo = 0; grainNo < grainCount; ++grainNo) {
      size_t srcOffset = chunkNo * chunkSize + grainNo * grainSize;
      size_t dstOffset = chunkNo * chunkSize + grainNo * grainSize * data->factor;
      for(size_t i = 0; i < grainSize; ++i) {
        float t = (float)i / grainSize;
        dst[dstOffset + i] = src[srcOffset + i];
      }
    }
  }
}

static void grainStretchDestroy(Processor *processor) {
  free(processor->data);
}

Processor mkGrainStretchProcessor(void) {
  Processor processor;
  processor.data = (GrainStretchData *)malloc(sizeof(GrainStretchData));
  processor.setParam = grainStretchSetParam;
  processor.bufferSize = grainStretchBufferSize;
  processor.process = grainStretchProcess;
  processor.destroy = grainStretchDestroy;

  GrainStretchData *data = processor.data;
  data->factor = 1.0;
  data->granularity = 0.1;

  return processor;
}
