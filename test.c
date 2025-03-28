#include "core/sample.h"
#include "core/source.h"
#include "core/processor.h"

static Buffer applyProcessor(const Buffer *src, Processor *processor) {
  size_t bufferLength = processor->bufferSize(processor, src->length);
  Buffer dst = mkBuffer(src->sampleRate, bufferLength);
  processorProcess(processor, src, &dst);
  return dst;
}

static void saveBufferToFile(const Buffer *buffer, const char *filename) {
  FILE *file = fopen(filename, "wb");
  saveBuffer(buffer, file);
  fclose(file);
}

int main() {
  int sampleRate = DEFAULT_SAMPLE_RATE;

  Source source = mkSource("sine");
  sourceSetParam(&source, "duration", 0.05);
  sourceSetParam(&source, "freq", 2400.0);
  sourceSetParam(&source, "amplitude", 0.5);

  size_t bufferLength = source.bufferSize(&source, sampleRate);
  Buffer raw = mkBuffer(sampleRate, bufferLength);
  sourceFillBuffer(&source, &raw);
  saveBufferToFile(&raw, "test/raw.wav");

  Processor fadeOut = mkProcessor("fadeOut");
  processorSetParam(&fadeOut, "duration", 0.25);
  Buffer fadedOut = applyProcessor(&raw, &fadeOut);

  Processor clip = mkProcessor("hardClip");
  processorSetParam(&clip, "threshold", 0.5);

  Processor stretch = mkProcessor("interpStretch");
  processorSetParam(&stretch, "factor", 10.0);

  processorSetParam(&stretch, "interp", 0.0);
  Buffer interp0 = applyProcessor(&raw, &stretch);
  Buffer interp0Clipped = applyProcessor(&interp0, &clip);
  Buffer interp0Faded = applyProcessor(&interp0Clipped, &fadeOut);
  saveBufferToFile(&interp0Faded, "test/interp0.wav");
  destroyBuffer(&interp0Faded);
  destroyBuffer(&interp0Clipped);
  destroyBuffer(&interp0);

  processorSetParam(&stretch, "interp", 1.0);
  Buffer interp1 = applyProcessor(&raw, &stretch);
  Buffer interp1Clipped = applyProcessor(&interp1, &clip);
  Buffer interp1Faded = applyProcessor(&interp1Clipped, &fadeOut);
  saveBufferToFile(&interp1Faded, "test/interp1.wav");
  destroyBuffer(&interp1Faded);
  destroyBuffer(&interp1Clipped);
  destroyBuffer(&interp1);

  processorSetParam(&stretch, "interp", 2.0);
  Buffer interp2 = applyProcessor(&raw, &stretch);
  Buffer interp2Clipped = applyProcessor(&interp2, &clip);
  Buffer interp2Faded = applyProcessor(&interp2Clipped, &fadeOut);
  saveBufferToFile(&interp2Faded, "test/interp2.wav");
  destroyBuffer(&interp2Faded);
  destroyBuffer(&interp2Clipped);
  destroyBuffer(&interp2);

  processorSetParam(&stretch, "interp", 3.0);
  Buffer interp3 = applyProcessor(&raw, &stretch);
  Buffer interp3Clipped = applyProcessor(&interp3, &clip);
  Buffer interp3Faded = applyProcessor(&interp3Clipped, &fadeOut);
  saveBufferToFile(&interp3Faded, "test/interp3.wav");
  destroyBuffer(&interp3Faded);
  destroyBuffer(&interp3Clipped);
  destroyBuffer(&interp3);

  processorDestroy(&stretch);
  processorDestroy(&clip);
  processorDestroy(&fadeOut);
  destroyBuffer(&raw);
  sourceDestroy(&source);

  return 0;
}
