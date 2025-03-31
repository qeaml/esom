#include "source.h"

#include <stdlib.h>

typedef struct {
  float duration;
  float freq;
  float phase;
  float amplitude;
} SquareData;

static void squareSetFloatParam(Source *source, const char *param, float value) {
  SquareData *data = source->data;
  SOURCE_PARAM(data, duration);
  SOURCE_PARAM(data, freq);
  SOURCE_PARAM(data, phase);
  SOURCE_PARAM(data, amplitude);
}

static void squareSetStringParam(Source *source, const char *param, const char *value) {
}

static size_t squareBufferSize(Source *source, int sampleRate) {
  SquareData *data = source->data;
  return (size_t)(sampleRate * data->duration);
}

/* generate a square wave in the buffer using SquareData in source */
static void squareFillBuffer(Source *source, Buffer *buffer) {
  SquareData *data = source->data;
  size_t length = buffer->length;
  Sample *samples = buffer->samples;
  float freq = data->freq;
  float phase = data->phase;
  float amplitude = data->amplitude;
  int cycleSamples = buffer->sampleRate / freq;
  for (size_t i = 0; i < length; i++) {
    float value = phase < cycleSamples / 2 ? amplitude : -amplitude;
    samples[i].left = value;
    samples[i].right = value;
    phase++;
    if (phase >= cycleSamples) {
      phase = 0;
    }
  }
}

static void squareDestroy(Source *source) {
  free(source->data);
}

Source mkSquareSource() {
  Source source;
  source.setFloatParam = squareSetFloatParam;
  source.setStringParam = squareSetStringParam;
  source.bufferSize = squareBufferSize;
  source.fillBuffer = squareFillBuffer;
  source.destroy = squareDestroy;
  source.data = malloc(sizeof(SquareData));
  SquareData *data = source.data;
  data->duration = 1.0f;
  data->freq = 440.0f;
  data->phase = 0.0f;
  data->amplitude = 1.0f;
  return source;
}
