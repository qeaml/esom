#include "core/sample.h"
#include "core/source.h"
#include "core/processor.h"

static Buffer applyProcessor(Buffer *buffer, Processor *processor) {
  size_t bufferLength = processor->bufferSize(processor, buffer->length);
  if(bufferLength < buffer->length) {
    buffer->length = bufferLength;
    processorProcess(processor, buffer);
    return emptyBuffer();
  }
  if(bufferLength == buffer->length) {
    processorProcess(processor, buffer);
    return emptyBuffer();
  }
  Buffer newBuffer = mkBuffer(bufferLength, buffer->sampleRate);
  copyBuffer(&newBuffer, buffer);
  processorProcess(processor, buffer);
  return newBuffer;
}

static void saveBufferToFile(const Buffer *buffer, const char *filename) {
  FILE *file = fopen(filename, "wb");
  saveBuffer(buffer, file);
  fclose(file);
}

int main() {
  int sampleRate = DEFAULT_SAMPLE_RATE;

  Source source = mkSource("sine");
  sourceSetParam(&source, "duration", 1.5);
  sourceSetParam(&source, "freq", 500.0);
  sourceSetParam(&source, "amplitude", 0.9);

  Processor gain = mkProcessor("gain");
  processorSetParam(&gain, "gain", 1.3);

  Processor hardClip = mkProcessor("hardClip");
  processorSetParam(&hardClip, "threshold", 0.9);
  processorSetParam(&hardClip, "synched", 1);

  size_t bufferLength = source.bufferSize(&source, sampleRate);
  Buffer buffer = mkBuffer(sampleRate, bufferLength);
  sourceFillBuffer(&source, &buffer);
  saveBufferToFile(&buffer, "sine.wav");

  applyProcessor(&buffer, &gain);
  saveBufferToFile(&buffer, "sine_gain.wav");
  applyProcessor(&buffer, &hardClip);
  saveBufferToFile(&buffer, "sine_gain_hard_clip.wav");

  destroyBuffer(&buffer);
  sourceDestroy(&source);
  processorDestroy(&gain);
  processorDestroy(&hardClip);

  return 0;
}
