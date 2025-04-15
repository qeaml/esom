#include "processor.h"

#include <math.h>
#include <string.h>

typedef enum {
  FormulaDyn,  // x = (x*x*x + 2*sqrt(x))/3
  FormulaSin,  // x = sin((pi*x)/2)
  FormulaCbrt, // x = cbrt(x)
} Formula;

#define PROCESSOR_FORMULA(processor) \
  ((Formula*)&processor->data)

#define NAMED_FORMULA(formulaName)                        \
  if(strcmp(value, #formulaName) == 0) {                  \
    *PROCESSOR_FORMULA(processor) = Formula##formulaName; \
    return;                                               \
  }

static void formulaSetStringParam(Processor *processor, const char *param, const char *value) {
  if(strcmp(param, "formula") != 0) {
    return;
  }
  NAMED_FORMULA(Dyn);
  NAMED_FORMULA(Sin);
  NAMED_FORMULA(Cbrt);
}

__attribute__((pure))
static float formulaDyn(float x) {
  return (x*x*x + 2*sqrtf(x))/3;
}

__attribute__((pure))
static float formulaSin(float x) {
  return sinf((M_PI*x)/2);
}

__attribute__((pure))
static float formulaCbrt(float x) {
  return cbrtf(x);
}

static size_t formulaBufferSize(Processor *processor, size_t inputSize) {
  return inputSize;
}

#define CASE(name)                                                  \
  case Formula##name:                                               \
    for(size_t i = 0; i < src->length; ++i) {                       \
      dst->samples[i].left = formula##name(src->samples[i].left);   \
      dst->samples[i].right = formula##name(src->samples[i].right); \
    }                                                               \
    break;

static void formulaProcess(Processor *processor, const Buffer *src, Buffer *dst) {
  switch(*PROCESSOR_FORMULA(processor)) {
  CASE(Dyn)
  CASE(Sin)
  CASE(Cbrt)
  }
}

static void formulaDestroy(Processor *processor) {
}

Processor mkFormulaProcessor(void) {
  Processor processor = {
    .data = NULL,
    .setFloatParam = noopSetFloatParam,
    .setStringParam = formulaSetStringParam,
    .setBufferParam = noopSetBufferParam,
    .bufferSize = formulaBufferSize,
    .process = formulaProcess,
    .destroy = formulaDestroy,
  };
  return processor;
}
