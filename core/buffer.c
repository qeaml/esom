#include "sample.h"
#include <stdlib.h>

Buffer emptyBuffer() {
  Buffer buffer;
  buffer.sampleRate = DEFAULT_SAMPLE_RATE;
  buffer.samples = NULL;
  buffer.length = 0;
  return buffer;
}

Buffer mkBuffer(int sampleRate, size_t length) {
  Buffer buffer;
  buffer.sampleRate = sampleRate;
  buffer.samples = malloc(length * sizeof(Sample));
  buffer.length = length;
  return buffer;
}

void destroyBuffer(Buffer *buffer) {
  if(buffer->samples == NULL) {
    return;
  }
  free(buffer->samples);
}

void copyBuffer(Buffer *dest, const Buffer *src) {
  dest->sampleRate = src->sampleRate;
  size_t length = src->length;
  if(length > dest->length) {
    length = dest->length;
  }
  for(size_t i = 0; i < length; i++) {
    dest->samples[i] = src->samples[i];
  }
}

void zeroBuffer(Buffer *buffer) {
  for(size_t i = 0; i < buffer->length; i++) {
    buffer->samples[i].left = 0.0;
    buffer->samples[i].right = 0.0;
  }
}

/* save the buffer as a WAV file */
void saveBuffer(const Buffer *buffer, FILE *file) {
  fwrite("RIFF", 1, 4, file);
  unsigned dataSize = buffer->length * sizeof(Sample);
  unsigned fileSize = 36 + dataSize;
  fwrite(&fileSize, 4, 1, file);
  fwrite("WAVE", 1, 4, file);

  fwrite("fmt ", 1, 4, file);
  unsigned fmtSize = 16;
  fwrite(&fmtSize, 4, 1, file);
  unsigned short format = 3;
  fwrite(&format, 2, 1, file);
  unsigned short channels = 2;
  fwrite(&channels, 2, 1, file);
  unsigned samplesPerSec = buffer->sampleRate;
  fwrite(&samplesPerSec, 4, 1, file);
  unsigned short bytePerBlock = sizeof(Sample);
  unsigned bytePerSec = samplesPerSec * bytePerBlock;
  fwrite(&bytePerSec, 4, 1, file);
  fwrite(&bytePerBlock, 2, 1, file);
  unsigned short bitsPerSample = 32;
  fwrite(&bitsPerSample, 2, 1, file);

  fwrite("data", 1, 4, file);
  fwrite(&dataSize, 4, 1, file);
  fwrite(buffer->samples, sizeof(Sample), buffer->length, file);
}
