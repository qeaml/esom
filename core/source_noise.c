#include "source.h"

#include <stdlib.h>

typedef struct {
  float duration;
  float amplitude;
} NoiseData;

static void noiseSetParam(Source *source, const char *name, float value) {
  NoiseData *data = source->data;
  SOURCE_PARAM(data, duration);
  SOURCE_PARAM(data, amplitude);
}

static size_t noiseBufferSize(Source *source, int sampleRate) {
  NoiseData *data = source->data;
  return sampleRate * data->duration;
}

/* generate white noise in the buffer using NoiseData in source */
static void noiseFillBuffer(Source *source, Buffer *buffer) {
  NoiseData *data = source->data;
  size_t length = buffer->length;
  Sample *samples = buffer->samples;
  for(size_t i = 0; i < length; i++) {
    samples[i].left = (float)rand() / RAND_MAX * 2.0f - 1.0f;
    samples[i].right = (float)rand() / RAND_MAX * 2.0f - 1.0f;
  }
}

static void noiseDestroy(Source *source) {
  free(source->data);
}

Source mkSourceNoise() {
  Source source;
  source.setParam = noiseSetParam;
  source.bufferSize = noiseBufferSize;
  source.fillBuffer = noiseFillBuffer;
  source.destroy = noiseDestroy;
  source.data = malloc(sizeof(NoiseData));
  NoiseData *data = source.data;
  data->duration = 1.0f;
  data->amplitude = 1.0f;
  return source;
}
