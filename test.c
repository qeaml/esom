#include "core/sample.h"
#include "core/source.h"
#include "core/processor.h"

static Buffer applyProcessor(const Buffer *src, Processor *processor) {
  size_t bufferLength = processor->bufferSize(processor, src->length);
  Buffer dst = mkBuffer(bufferLength, src->sampleRate);
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
  sourceSetParam(&source, "duration", 0.1);
  sourceSetParam(&source, "freq", 500.0);
  sourceSetParam(&source, "amplitude", 0.9);

  Processor processor = mkProcessor("glitchStretch");
  processorSetParam(&processor, "factor", 10.0);

  size_t bufferLength = source.bufferSize(&source, sampleRate);
  Buffer src = mkBuffer(sampleRate, bufferLength);
  sourceFillBuffer(&source, &src);
  saveBufferToFile(&src, "test/raw.wav");
  Buffer dst = applyProcessor(&src, &processor);
  saveBufferToFile(&dst, "test/processed.wav");
  destroyBuffer(&dst);

  destroyBuffer(&src);
  sourceDestroy(&source);
  processorDestroy(&processor);

  return 0;
}
