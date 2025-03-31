#include "sndfile_handler.h"
#include <dlfcn.h>

int loadLibSndFile(LibSndFile *sndFile) {
  sndFile->lib = dlopen("libsndfile.so", RTLD_LAZY);
  if(!sndFile->lib) {
    return 0;
  }
  sndFile->open = (SfOpenPfn)dlsym(sndFile->lib, "sf_open");
  sndFile->error = (SfErrorPfn)dlsym(sndFile->lib, "sf_error");
  sndFile->readFloat = (SfReadFloatPfn)dlsym(sndFile->lib, "sf_read_float");
  sndFile->close = (SfClosePfn)dlsym(sndFile->lib, "sf_close");
  if(!sndFile->open || !sndFile->error || !sndFile->readFloat || !sndFile->close) {
    return 0;
  }
  return 1;
}

void unloadLibSndFile(LibSndFile *sndFile) {
  dlclose(sndFile->lib);
}
