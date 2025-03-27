#include "processor.h"

void processorSetParam(Processor *processor, const char *param, float value) {
  processor->setParam(processor, param, value);
}

size_t processorBufferSize(Processor *processor, size_t inputSize) {
  return processor->bufferSize(processor, inputSize);
}

void processorProcess(Processor *processor, const Buffer *src, Buffer *dst) {
  processor->process(processor, src, dst);
}

void processorDestroy(Processor *processor) {
  processor->destroy(processor);
}

#define PROCESSOR(processorName, factory) \
  if(strcmp(name, #processorName) == 0) { \
    return factory();                     \
  }

Processor mkProcessor(const char *name) {
  PROCESSOR(hardClip, mkHardClipProcessor);
  PROCESSOR(gain, mkGainProcessor);
  PROCESSOR(glitchStretch, mkGlitchStretchProcessor);
  return (Processor){NULL};
}
