#pragma once

#include "g_defs.h"
#include "g_error.h"

namespace gluon {

// Filesystem
namespace fs {

  class File {
  public:
    File();
    File(void *);
    ~File();

    MaybeError open(const Str &name);
    Result<word_t> size();
    Result<word_t> read(u8_t *dst, word_t bytes);
    inline bool is_good() { return !(is_error() || is_eof()); }
    bool is_error();
    bool is_eof();

  private:
    void *m_handle;
  };

} // ns fs

} // ns gluon