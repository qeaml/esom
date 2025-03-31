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

  Source source = mkSource("Noise");
  sourceSetParam(&source, "duration", 0.05);
  sourceSetParam(&source, "freq", 1760.0);
  sourceSetParam(&source, "amplitude", 0.9);

  size_t bufferLength = source.bufferSize(&source, sampleRate);
  Buffer raw = mkBuffer(sampleRate, bufferLength);
  sourceFillBuffer(&source, &raw);
  sourceDestroy(&source);

  Processor interp = mkProcessor("InterpStretch");
  processorSetParam(&interp, "factor", 4.0f);
  // 3.0f -> INTERP_BIT_FUCK
  // 4.0f -> INTERP_SMOOTHSTEP
  processorSetParam(&interp, "interp", 4.0f);
  Buffer stretched = applyProcessor(&raw, &interp);
  saveBufferToFile(&stretched, "test/stretched.wav");
  processorDestroy(&interp);
  destroyBuffer(&raw);

  Processor average = mkProcessor("Average");
  processorSetParam(&average, "bigWindow", 1);
  Buffer averaged = applyProcessor(&stretched, &average);
  saveBufferToFile(&averaged, "test/averaged.wav");
  processorDestroy(&average);
  destroyBuffer(&averaged);

  Processor power = mkProcessor("Power");
  processorSetParam(&power, "exponent", 3.0f);
  Buffer powered = applyProcessor(&stretched, &power);
  saveBufferToFile(&powered, "test/powered.wav");
  destroyBuffer(&powered);

  destroyBuffer(&stretched);
  return 0;
}
