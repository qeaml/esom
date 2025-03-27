#include "processor.h"

#include <stdlib.h>

typedef struct HardClipDataS {
  float threshold;
  int synched;
} HardClipData;

static void hardClipSetParam(Processor *processor, const char *name, float value) {
  HardClipData *data = (HardClipData *)processor->data;
  PROCESSOR_PARAM(data, threshold);
  PROCESSOR_PARAM_BOOL(data, synched);
}

static size_t hardClipBufferSize(Processor *processor, size_t inputSize) {
  return inputSize;
}

static void hardClipProcess(Processor *processor, Buffer *buffer) {
  HardClipData *data = (HardClipData *)processor->data;
  for(size_t i = 0; i < buffer->length; i++) {
    Sample *sample = &buffer->samples[i];
    if(data->synched) {
      if(sample->left > data->threshold || sample->right > data->threshold) {
        sample->left = data->threshold;
        sample->right = data->threshold;
      }
    } else {
      if(sample->left > data->threshold) {
        sample->left = data->threshold;
      }
      if(sample->right > data->threshold) {
        sample->right = data->threshold;
      }
    }
  }
}

static void hardClipDestroy(Processor *processor) {
  free(processor->data);
}

Processor mkHardClipProcessor(void) {
  Processor processor;
  processor.setParam = hardClipSetParam;
  processor.bufferSize = hardClipBufferSize;
  processor.process = hardClipProcess;
  processor.destroy = hardClipDestroy;
  processor.data = malloc(sizeof(HardClipData));
  HardClipData *data = (HardClipData *)processor.data;
  data->threshold = 0.5;
  data->synched = 0;
  return processor;
}
