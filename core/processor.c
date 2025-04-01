#include "processor.h"

void processorSetFloatParam(Processor *processor, const char *param, float value) {
  processor->setFloatParam(processor, param, value);
}

void processorSetStringParam(Processor *processor, const char *param, const char *value) {
  processor->setStringParam(processor, param, value);
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

#define PROCESSOR_MK_FUNC(name) \
  Processor mk##name##Processor(void);

PROCESSORS(PROCESSOR_MK_FUNC)

#define PROCESSOR_MK(processorName)        \
  if(strcmp(name, #processorName) == 0) {  \
    return mk##processorName##Processor(); \
  }

Processor mkProcessor(const char *name) {
  PROCESSORS(PROCESSOR_MK)
  return (Processor){NULL};
}
