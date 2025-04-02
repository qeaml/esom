#include "processor.h"

#include <stdlib.h>

typedef enum {
  InterpLinear,
  InterpPow2,
  InterpInvLinear,
  InterpBitFuck,
  InterpSmoothstep
} Interp;

typedef struct {
  float factor;
  Interp interp;
} StretchData;

static void interpStretchSetFloatParam(Processor *processor, const char *param, float value) {
  StretchData *data = processor->data;
  PROCESSOR_PARAM(data, factor);
}

#define CASE_INTERP(interpName)         \
  if(strcmp(value, #interpName) == 0) { \
    data->interp = Interp##interpName;  \
    return;                             \
  }

static void interpStretchSetStringParam(Processor *processor, const char *name, const char *value) {
  if(strcmp(name, "interp") != 0) {
    return;
  }
  StretchData *data = processor->data;
  CASE_INTERP(Linear)
  CASE_INTERP(Pow2)
  CASE_INTERP(InvLinear)
  CASE_INTERP(Smoothstep)
}

static size_t interpStretchBufferSize(Processor *processor, size_t inputSize) {
  StretchData *data = processor->data;
  return inputSize * data->factor;
}

static float linearInterp(float a, float b, float t) {
  return a + (b - a) * t;
}

static Sample linearInterpSample(Sample a, Sample b, float t) {
  Sample result;
  result.left = linearInterp(a.left, b.left, t);
  result.right = linearInterp(a.right, b.right, t);
  return result;
}

static float pow2Interp(float a, float b, float t) {
  return a + (b - a) * t * t;
}

static Sample pow2InterpSample(Sample a, Sample b, float t) {
  Sample result;
  result.left = pow2Interp(a.left, b.left, t);
  result.right = pow2Interp(a.right, b.right, t);
  return result;
}

static float invLinearInterp(float a, float b, float t) {
  return (t - a) / (b - a);
}

static Sample invLinearInterpSample(Sample a, Sample b, float t) {
  Sample result;
  result.left = invLinearInterp(a.left, b.left, t);
  result.right = invLinearInterp(a.right, b.right, t);
  return result;
}

static float bitFuckInterp(float a, float b, float t) {
  float preciousValue = t * (a + b);
  unsigned preciousBits = *(unsigned*)&preciousValue;
  if(preciousBits & 1) {
    return a;
  } else {
    return b;
  }
}

static Sample bitFuckInterpSample(Sample a, Sample b, float t) {
  Sample result;
  result.left = bitFuckInterp(a.left, b.left, t);
  result.right = bitFuckInterp(a.right, b.right, t);
  return result;
}

static float smoothstepInterp(float a, float b, float t) {
  float x = t * t * (3.0f - 2.0f * t);
  return a + (b - a) * x;
}

static Sample smoothstepInterpSample(Sample a, Sample b, float t) {
  Sample result;
  result.left = smoothstepInterp(a.left, b.left, t);
  result.right = smoothstepInterp(a.right, b.right, t);
  return result;
}

#define CASE(interpName, interpFunc)                   \
  case Interp##interpName:                             \
    dst->samples[i] = interpFunc(sampleA, sampleB, t); \
    break;

static void interpStretchProcess(Processor *processor, const Buffer *src, Buffer *dst) {
  StretchData *data = processor->data;
  for(size_t i = 0; i < dst->length; ++i) {
    size_t srcIdx = i / data->factor;
    Sample sampleA = src->samples[srcIdx];
    Sample sampleB;
    if(srcIdx == src->length - 1) {
      sampleB.left = 0.0f;
      sampleB.right = 0.0f;
    } else {
      sampleB = src->samples[srcIdx + 1];
    }
    float t = (float)i / data->factor - srcIdx;
    switch(data->interp) {
    CASE(Linear, linearInterpSample)
    CASE(Pow2, pow2InterpSample)
    CASE(InvLinear, invLinearInterpSample)
    CASE(BitFuck, bitFuckInterpSample)
    CASE(Smoothstep, smoothstepInterpSample)
    }
  }
}

static void interpStretchDestroy(Processor *processor) {
  free(processor->data);
}

Processor mkInterpStretchProcessor(void) {
  Processor processor = {
    .data = calloc(1, sizeof(StretchData)),
    .setFloatParam = interpStretchSetFloatParam,
    .setStringParam = interpStretchSetStringParam,
    .bufferSize = interpStretchBufferSize,
    .process = interpStretchProcess,
    .destroy = interpStretchDestroy
  };
  return processor;
}
