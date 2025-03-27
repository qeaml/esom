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
  sourceSetParam(&source, "duration", 0.02);
  sourceSetParam(&source, "freq", 2000.0);
  sourceSetParam(&source, "amplitude", 0.9);

  Processor processor = mkProcessor("glitchStretch");
  processorSetParam(&processor, "factor", 100.0);
  Processor fadeOut = mkProcessor("fadeOut");
  processorSetParam(&fadeOut, "duration", 0.5);
  Processor fadeIn = mkProcessor("fadeIn");
  processorSetParam(&fadeIn, "duration", 0.5);
  Processor clipper = mkProcessor("hardClip");
  processorSetParam(&clipper, "threshold", 0.5);

  size_t bufferLength = source.bufferSize(&source, sampleRate);
  Buffer src = mkBuffer(sampleRate, bufferLength);
  sourceFillBuffer(&source, &src);
  saveBufferToFile(&src, "test/raw.wav");
  Buffer dst = applyProcessor(&src, &processor);
  destroyBuffer(&src);
  Buffer dst2 = applyProcessor(&dst, &clipper);
  destroyBuffer(&dst);
  Buffer dst3 = applyProcessor(&dst2, &fadeOut);
  destroyBuffer(&dst2);
  Buffer dst4 = applyProcessor(&dst3, &fadeIn);
  destroyBuffer(&dst3);
  saveBufferToFile(&dst4, "test/processed.wav");
  destroyBuffer(&dst4);

  sourceDestroy(&source);
  processorDestroy(&processor);

  return 0;
}
