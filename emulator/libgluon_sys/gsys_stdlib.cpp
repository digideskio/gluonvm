#include "g_sys_stdlib.h"
#include "g_error.h"

// Ha! Cheat by using standard library!
#include <stdlib.h>

namespace gluon {
namespace stdlib {

  void abort() {
    ::abort();
  }

} // ns mem
} // ns gluon
