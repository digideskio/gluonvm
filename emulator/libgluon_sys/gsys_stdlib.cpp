#include "g_sys_stdlib.h"
#include "g_error.h"

#include <stdlib.h>
#include <unistd.h>

namespace gluon {
namespace Std {

  void abort() {
    ::abort();
  }

  void sleep(word_t micro_sec) {
    ::usleep(micro_sec);
  }

  void exit(int x) {
    ::exit(x);
  }
} // ns mem
} // ns gluon
