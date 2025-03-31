#include "processor.h"

#include <stdlib.h>

#define INTERP_LINEAR 0.0f
#define INTERP_POW2 1.0f
#define INTERP_INV_LINEAR 2.0f
#define INTERP_BIT_FUCK 3.0f
#define INTERP_SMOOTHSTEP 4.0f

typedef struct {
  float factor;
  float interp;
} StretchData;

static void interStretchSetParam(Processor *processor, const char *name, float value) {
  StretchData *data = processor->data;
  PROCESSOR_PARAM(data, factor);
  PROCESSOR_PARAM(data, interp);
}

static size_t interStretchBufferSize(Processor *processor, size_t inputSize) {
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



static void interStretchProcess(Processor *processor, const Buffer *src, Buffer *dst) {
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
    #define DO(interp_value, interp_func)                   \
      if(data->interp == interp_value) {                    \
        dst->samples[i] = interp_func(sampleA, sampleB, t); \
      }
    DO(INTERP_LINEAR, linearInterpSample)
    DO(INTERP_POW2, pow2InterpSample)
    DO(INTERP_INV_LINEAR, invLinearInterpSample)
    DO(INTERP_BIT_FUCK, bitFuckInterpSample)
    DO(INTERP_SMOOTHSTEP, smoothstepInterpSample)
  }
}

static void interStretchDestroy(Processor *processor) {
  free(processor->data);
}

Processor mkInterpStretchProcessor(void) {
  Processor processor;
  processor.setParam = interStretchSetParam;
  processor.bufferSize = interStretchBufferSize;
  processor.process = interStretchProcess;
  processor.destroy = interStretchDestroy;
  StretchData *data = malloc(sizeof(StretchData));
  data->factor = 1.0f;
  data->interp = INTERP_LINEAR;
  processor.data = data;
  return processor;
}
