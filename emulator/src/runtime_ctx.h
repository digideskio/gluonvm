#pragma once

#include "term.h"

namespace gluon {
namespace erts {

// Set of stuff we take from Process struct to keep running, this will be saved
// by loop runner on context switch or loop end
class RuntimeContextFields {
private:
  //  Module *mod = nullptr;
  const Word* ip_ = nullptr;
  // continuation, works like return address for a single call. If more nested
  // calls are done, cp is saved to stack
  const Word* cp_ = nullptr;

public:
  void set_ip(const Word* value) {
    G_ASSERT((Word)value > 0xffff); // some sane minimum for a pointer
    G_ASSERT(value);
    ip_ = value;
  }
  const Word* ip() const { return ip_; }
  Word ip(Word index) const { return ip_[index]; }
  void step_ip(SWord offset) { ip_ += offset; }
  void inc_ip() { ip_++; }

  void set_cp(const Word* value) {
    // some sane minimum for a pointer or nullptr
    G_ASSERT(!value || (Word)value > 0xffff);
    cp_ = value;
  }
  const Word* cp() const { return cp_; }

  Word live = 0;  // saved registers count

  // TODO: maybe cache r0 in a local variable in vm loop?
  Term regs[erts::max_regs];
#if FEATURE_FLOAT
// Float fp_regs[vm::MAX_FP_REGS];
#endif
};

class RuntimeContext: public RuntimeContextFields {
#if G_DEBUG
private:
  enum class ContextBelongsTo {
    VmLoop,         // belongs to VM loop, do not modify ctx fields now
    ProcessPartial, // lightly swapped out (only ip/cp)
    Process,        // fully swapped out with registers etc
  };

  // Extra debug-time check to see if ctx belongs to VM or is swapped out
  ContextBelongsTo belongs_ = ContextBelongsTo::Process;

public:
  void assert_swapped_out() {
    G_ASSERT(belongs_ == ContextBelongsTo::Process);
  }
  void assert_swapped_out_partial() {
    G_ASSERT(belongs_ == ContextBelongsTo::ProcessPartial
             || belongs_ == ContextBelongsTo::Process);
  }
  void assert_swapped_in() {
    G_ASSERT(belongs_ == ContextBelongsTo::VmLoop);
  }
  void swapped_out() {
    assert_swapped_in();
    belongs_ = ContextBelongsTo::Process;
  }
  void swapped_out_partial() {
    assert_swapped_in();
    belongs_ = ContextBelongsTo::ProcessPartial;
  }
  void swapped_in() {
    assert_swapped_out_partial();
    belongs_ = ContextBelongsTo::VmLoop;
  }
#else
public:
  void assert_swapped_out() {}
  void assert_swapped_out_partial() {}
  void assert_swapped_in() {}
  void swapped_out() {}
  void swapped_out_partial() {}
  void swapped_in() {}
#endif
public:
  // Entry arguments for apply
  constexpr static Word num_arg_regs = 6;
  Term arg_regs_[num_arg_regs];
};

} // ns erts
} // ns gluon