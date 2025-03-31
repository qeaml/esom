#include "sample.h"
#include <sndfile.h>

typedef SNDFILE*(*SfOpenPfn)(const char*, int, SF_INFO*);
typedef int(*SfErrorPfn)(SNDFILE*);
typedef sf_count_t(*SfReadFloatPfn)(SNDFILE*, float*, sf_count_t);
typedef int(*SfClosePfn)(SNDFILE*);

typedef struct {
  void *lib;
  SfOpenPfn open;
  SfErrorPfn error;
  SfReadFloatPfn readFloat;
  SfClosePfn close;
} LibSndFile;

int loadLibSndFile(LibSndFile *sndFile);
void unloadLibSndFile(LibSndFile *sndFile);

int loadSound(Buffer *out, const char *path, LibSndFile *sndFile);
