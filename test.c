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

int main(int argc, char **argv) {
  int sampleRate = DEFAULT_SAMPLE_RATE;

  if(argc < 2) {
    return 0;
  }

  Source source = mkSource("Sample");
  sourceSetParam(&source, "path", argv[1]);

  size_t bufferLength = source.bufferSize(&source, sampleRate);
  Buffer raw = mkBuffer(sampleRate, bufferLength);
  sourceFillBuffer(&source, &raw);
  sourceDestroy(&source);

  Processor interp = mkProcessor("InterpStretch");
  processorSetParam(&interp, "factor", 4.0f);
  processorSetParam(&interp, "interp", "BitFuck");
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
  processorSetParam(&power, "exponent", 2.0f);
  Buffer powered = applyProcessor(&stretched, &power);
  saveBufferToFile(&powered, "test/powered.wav");
  destroyBuffer(&powered);

  Processor formula = mkProcessor("Formula");
  processorSetParam(&formula, "formula", "Cbrt");
  Buffer calculated = applyProcessor(&stretched, &formula);
  saveBufferToFile(&calculated, "test/calculated.wav");
  processorDestroy(&formula);

  destroyBuffer(&stretched);
  return 0;
}
