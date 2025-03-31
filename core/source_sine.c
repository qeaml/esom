#include "source.h"

#include <math.h>
#include <stdlib.h>

typedef struct {
  float duration;
  float freq;
  float phase;
  float amplitude;
} SineData;

static void sineSetFloatParam(Source *source, const char *param, float value) {
  SineData *data = source->data;
  SOURCE_PARAM(data, duration);
  SOURCE_PARAM(data, freq);
  SOURCE_PARAM(data, phase);
  SOURCE_PARAM(data, amplitude);
}

static void sineSetStringParam(Source *source, const char *param, const char *value) {
}

static size_t sineBufferSize(Source *source, int sampleRate) {
  SineData *data = source->data;
  return (size_t)(sampleRate * data->duration);
}

/* generate a sine wave in the buffer using SineData in source */
static void sineFillBuffer(Source *source, Buffer *buffer) {
  SineData *data = source->data;
  size_t length = buffer->length;
  Sample *samples = buffer->samples;
  float freq = data->freq;
  float phase = data->phase;
  float amplitude = data->amplitude;
  float phaseIncrement = 2.0f * M_PI * freq / buffer->sampleRate;
  for (size_t i = 0; i < length; i++) {
    float value = sinf(phase) * amplitude;
    samples[i].left = value;
    samples[i].right = value;
    phase += phaseIncrement;
  }
}

static void sineDestroy(Source *source) {
  free(source->data);
}

Source mkSineSource() {
  Source source;
  source.setFloatParam = sineSetFloatParam;
  source.setStringParam = sineSetStringParam;
  source.bufferSize = sineBufferSize;
  source.fillBuffer = sineFillBuffer;
  source.destroy = sineDestroy;
  source.data = malloc(sizeof(SineData));
  SineData *data = source.data;
  data->duration = 1.0f;
  data->freq = 440.0f;
  data->phase = 0.0f;
  data->amplitude = 1.0f;
  return source;
}
