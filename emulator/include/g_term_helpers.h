#pragma once

#include "g_defs.h"
#include "g_term.h"
#include "g_heap.h"

namespace gluon {
namespace term {

class TupleBuilder {
  word_t m_arity;
  word_t m_index;
  Term *m_elements;
public:
  TupleBuilder(Heap *heap, word_t arity): m_arity(arity), m_index(1) {
    m_elements = Heap::alloc<Term>(heap, arity + 1);
  }
  inline void add(Term x) {
    G_ASSERT(m_index < m_arity);
    m_index++;
    m_elements[m_index] = x;
  }
  inline Term make_tuple() {
    G_ASSERT(m_index == m_arity)
    return Term::make_tuple(m_elements, m_arity);
  }
};

template <typename T> inline Term make_term(const T&);
template <> inline Term make_term(const char &x) {
  return Term::make_small_u((word_t)x);
}
template <> inline Term make_term(const word_t &x) {
  return Term::make_small_u(x);
}
template <> inline Term make_term(const sword_t &x) {
  return Term::make_small(x);
}

template <typename Iter> inline word_t length(Iter iter, Iter to) {
  word_t result = 0;
  for(; iter != to; iter++, result++) {
  }
  return result;
}

template <typename Iter>
Term build_list(Heap *heap, Iter iter, Iter to) {
  if (iter == to) {
    return ::gluon::NIL;
  }

  word_t len = length(iter, to);
  printf("len=%zu\n", len);
  Term *h = Heap::alloc<Term>(heap, 2 * len);

  Term result = Term::make_cons(h);
  word_t i = 0;
  for(; iter != to; iter++) {
    h[0] = make_term(*iter);
    h[1] = (i == len - 1) ? ::gluon::NIL : Term::make_cons(h + 2);
    h += 2;
    i++;
  }

  return result;
}

// Builds string as list of integers on heap
static inline Term build_string(Heap *heap, const Str &s) {
  return build_list(heap, s.begin(), s.end());
}

} // ns term
} // ns gluon