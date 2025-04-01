#include "processor.h"

#include <stdlib.h>

#define BIG_WINDOW_SPAN 5
#define BIG_WINDOW_SIZE (2 * BIG_WINDOW_SPAN + 1)
#define SMALL_WINDOW_SPAN 2
#define SMALL_WINDOW_SIZE (2 * SMALL_WINDOW_SPAN + 1)

typedef struct {
  /* if 0, use only 4 neighboring samples. uses provided weights. */
  /* if 1, use 10 neighboring samples instead. does not use weights. */
  int bigWindow;
  /* weights for averaging */
  float weights[SMALL_WINDOW_SIZE];
} AverageData;

static void averageSetFloatParam(Processor *processor, const char *name, float value) {
  AverageData *data = processor->data;
  PROCESSOR_PARAM_BOOL(data, bigWindow)
  PROCESSOR_PARAM(data, weights[0])
  PROCESSOR_PARAM(data, weights[1])
  PROCESSOR_PARAM(data, weights[2])
  PROCESSOR_PARAM(data, weights[3])
}

static void averageSetStringParam(Processor *processor, const char *name, const char *value) {
}

static size_t averageBufferSize(Processor *processor, size_t inputSize) {
  return inputSize;
}

static Sample get(const Buffer *buf, int index) {
  Sample sample;
  if(index < 0 || index >= buf->length) {
    sample.left = 0.0f;
    sample.right = 0.0f;
  } else {
    sample = buf->samples[index];
  }
  return sample;
}

static void averageProcessBigWindow(Processor *processor, const Buffer *src, Buffer *dst) {
  AverageData *data = processor->data;
  float total = BIG_WINDOW_SIZE;
  for(size_t i = 0; i < src->length; ++i) {
    float left = 0.0f;
    float right = 0.0f;
    for(int j = -BIG_WINDOW_SPAN; j <= BIG_WINDOW_SPAN; ++j) {
      Sample sample = get(src, i + j);
      left += sample.left;
      right += sample.right;
    }
    dst->samples[i].left = left / total;
    dst->samples[i].right = right / total;
  }
}

static void averageProcessSmallWindow(Processor *processor, const Buffer *src, Buffer *dst) {
  AverageData *data = processor->data;
  float total = 0.0f;
  for(int i = 0; i < SMALL_WINDOW_SIZE; ++i) {
    total += data->weights[i];
  }
  for(size_t i = 0; i < src->length; ++i) {
    float left = 0.0f;
    float right = 0.0f;
    for(int j = -SMALL_WINDOW_SPAN; j <= SMALL_WINDOW_SPAN; ++j) {
      Sample sample = get(src, i + j);
      left += sample.left * data->weights[j + SMALL_WINDOW_SPAN];
      right += sample.right * data->weights[j + SMALL_WINDOW_SPAN];
    }
    dst->samples[i].left = left / total;
    dst->samples[i].right = right / total;
  }
}

static void averageProcess(Processor *processor, const Buffer *src, Buffer *dst) {
  AverageData *data = processor->data;
  if(data->bigWindow) {
    averageProcessBigWindow(processor, src, dst);
  } else {
    averageProcessSmallWindow(processor, src, dst);
  }
}

static void averageDestroy(Processor *processor) {
  free(processor->data);
}

Processor mkAverageProcessor(void) {
  Processor processor = {
    .data = calloc(1, sizeof(AverageData)),
    .setFloatParam = averageSetFloatParam,
    .setStringParam = averageSetStringParam,
    .bufferSize = averageBufferSize,
    .process = averageProcess,
    .destroy = averageDestroy
  };
  return processor;
}
