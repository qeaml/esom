#include "processor.h"

#include <stdlib.h>

typedef struct GlitchStretchDataS {
  float factor;
} GlitchStretchData;

static void glitchStretchSetParam(Processor *processor, const char *name, float value) {
  GlitchStretchData *data = processor->data;
  PROCESSOR_PARAM(data, factor);
}

static size_t glitchStretchBufferSize(Processor *processor, size_t inputSize) {
  GlitchStretchData *data = processor->data;
  return inputSize * data->factor;
}

static void glitchStretchProcess(Processor *processor, const Buffer *src, Buffer *dst) {
  GlitchStretchData *data = processor->data;
  for(size_t i = 0; i < dst->length; ++i) {
    int stretched = (float)i / data->factor;
    dst->samples[i] = src->samples[stretched];
  }
}

static void glitchStretchDestroy(Processor *processor) {
  free(processor->data);
}

Processor mkGlitchStretchProcessor(void) {
  Processor processor;
  processor.setParam = glitchStretchSetParam;
  processor.bufferSize = glitchStretchBufferSize;
  processor.process = glitchStretchProcess;
  processor.destroy = glitchStretchDestroy;
  GlitchStretchData *data = malloc(sizeof(GlitchStretchData));
  data->factor = 1.0f;
  processor.data = data;
  return processor;
}
