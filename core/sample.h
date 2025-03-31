#pragma once

#include <stddef.h>
#include <stdio.h>

#define DEFAULT_SAMPLE_RATE 44100

typedef struct SampleS Sample;

struct SampleS {
  float left;
  float right;
};

typedef struct BufferS Buffer;

struct BufferS {
  int sampleRate;
  Sample *samples;
  size_t length;
};

Buffer emptyBuffer(void);
Buffer mkBuffer(int sampleRate, size_t length);
void destroyBuffer(Buffer *buffer);
void copyBuffer(Buffer *dest, const Buffer *src);
void zeroBuffer(Buffer *buffer);

void saveBuffer(const Buffer *buffer, FILE *file);