/*
e1p - Apply a processor to an audio file.

Usage:
    e1p <input file> <output file> <processor> [parameter=value ...]
*/

#include "../core/processor.h"
#include "../core/source.h"
#include <stdio.h>
#include <stdlib.h>

static void setParameter(Processor *processor, const char *raw) {
  char *value = strchr(raw, '=');
  if(value == NULL) {
    return;
  }
  *value = '\0';
  value++;
  if(value[0] == ':') {
    value++;
    fprintf(stderr, "%s = \"%s\"\n", raw, value);
    processorSetStringParam(processor, raw, value);
  } else {
    float floatValue = strtof(value, NULL);
    fprintf(stderr, "%s = %f\n", raw, floatValue);
    processorSetFloatParam(processor, raw, floatValue);
  }
}

int main(int argc, char **argv) {
  if(argc < 4) {
    fprintf(stderr, "Usage: %s <input file> <output file> <processor> [parameter=value ...]\n", argv[0]);
    return 1;
  }

  const char *inputFile = argv[1];
  const char *outputFile = argv[2];
  char *processorName = argv[3];

  Source source = mkSource("Sample");
  sourceSetParam(&source, "path", inputFile);

  size_t bufferLength = source.bufferSize(&source, DEFAULT_SAMPLE_RATE);
  if(bufferLength == 0) {
    fprintf(stderr, "Invalid input: %s\n", inputFile);
    return 1;
  }
  fprintf(stderr, "Input size: %zu\n", bufferLength);
  Buffer raw = mkBuffer(DEFAULT_SAMPLE_RATE, bufferLength);
  sourceFillBuffer(&source, &raw);
  sourceDestroy(&source);

  Processor processor = mkProcessor(processorName);
  if(processor.process == NULL) {
    fprintf(stderr, "Unknown processor: %s\n", processorName);
    destroyBuffer(&raw);
    return 1;
  }
  fprintf(stderr, "Processor: %s\n", processorName);
  for(int i = 4; i < argc; i++) {
    setParameter(&processor, argv[i]);
  }
  bufferLength = processor.bufferSize(&processor, raw.length);
  if(bufferLength == 0) {
    fprintf(stderr, "Invalid processor: %s\n", processorName);
    processorDestroy(&processor);
    destroyBuffer(&raw);
    return 1;
  }
  fprintf(stderr, "Output size: %zu\n", bufferLength);
  Buffer processed = mkBuffer(DEFAULT_SAMPLE_RATE, bufferLength);
  processorProcess(&processor, &raw, &processed);
  processorDestroy(&processor);
  destroyBuffer(&raw);

  FILE *file = fopen(outputFile, "wb");
  saveBuffer(&processed, file);
  fclose(file);

  destroyBuffer(&processed);
  return 0;
}
