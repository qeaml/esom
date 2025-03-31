#include "source.h"
#include "sndfile_handler.h"

#include <alloca.h>
#include <stdlib.h>

typedef struct {
  const char *path;
  SNDFILE *file;
  SF_INFO info;
} SampleData;

static void sampleSetFloatParam(Source *source, const char *param, float value) {
}

static void sampleSetStringParam(Source *source, const char *param, const char *value) {
  SampleData *data = source->data;
  SOURCE_PARAM(data, path);
}

static LibSndFile gSndFile;
static size_t gSampleSources = 0;

static size_t sampleBufferSize(Source *source, int sampleRate) {
  if(gSndFile.lib == NULL) {
    if(!loadLibSndFile(&gSndFile)) {
      return 0;
    }
  }
  SampleData *data = source->data;
  data->file = gSndFile.open(data->path, SFM_READ, &data->info);
  if(data->file == NULL) {
    return 0;
  }
  /* FIXME: resample? */
  if(data->info.samplerate != sampleRate) {
    gSndFile.close(data->file);
    return 0;
  }
  return (size_t)data->info.frames;
}

static void sampleFillBuffer(Source *source, Buffer *buffer) {
  SampleData *data = source->data;
  float *temp = alloca(data->info.channels * sizeof(float));
  for(size_t i = 0; i < buffer->length; ++i) {
    gSndFile.readFloat(data->file, temp, data->info.channels);
    buffer->samples[i].left = temp[0];
    if(data->info.channels == 1) {
      buffer->samples[i].right = temp[0];
    } else {
      buffer->samples[i].right = temp[1];
    }
  }
}

static void sampleDestroy(Source *source) {
  SampleData *data = source->data;
  if(data->file != NULL) {
    gSndFile.close(data->file);
  }
  free(data);

  if(gSampleSources > 0) {
    --gSampleSources;
    if(gSampleSources == 0 && gSndFile.lib != NULL) {
      unloadLibSndFile(&gSndFile);
    }
  }
}

Source mkSampleSource(const char *path) {
  Source source;
  source.setFloatParam = sampleSetFloatParam;
  source.setStringParam = sampleSetStringParam;
  source.bufferSize = sampleBufferSize;
  source.fillBuffer = sampleFillBuffer;
  source.destroy = sampleDestroy;
  source.data = malloc(sizeof(SampleData));
  SampleData *data = source.data;
  data->path = path;
  data->file = NULL;
  ++gSampleSources;
  return source;
}
