#include "g_defs.h"
#include "g_vm.h"
#include "g_ext_term.h"
#include "g_heap.h"

namespace gluon {

namespace err {
  DECL_EXCEPTION(ext_term_error)
  IMPL_EXCEPTION(ext_term_error)
} // ns err

namespace etf {

Term read_atom_string_i16(VM &vm, tool::Reader &r);
Term read_atom_string_i8(VM &vm, tool::Reader &r);
Term read_tagged_atom_string(VM &vm, tool::Reader &r);
Node *get_node(VM &vm, Term /*sysname*/, dist::creation_t /*creation*/);
Term make_pid(VM &vm, Term sysname, word_t id, word_t serial, u8_t creation);
Term read_tuple(VM &vm, proc::Heap *heap, tool::Reader &r, word_t arity);
Term read_string_ext(proc::Heap *heap, tool::Reader &r);
Term read_list_ext(VM &vm, proc::Heap *heap, tool::Reader &r);


// Reads long atom as string and attempts to create it in atom table.
Term read_atom_string_i16(VM &vm, tool::Reader &r) {
  word_t sz = r.read_big_u16();
  Str atom_str = r.read_string(sz);
  return vm.to_atom(atom_str);
}


// Reads short atom as string and attempts to create it in atom table.
Term read_atom_string_i8(VM &vm, tool::Reader &r) {
  word_t sz = r.read_byte();
  Str atom_str = r.read_string(sz);
  return vm.to_atom(atom_str);
}


// Reads tag byte, then reads long or short atom as string and attempts to
// create it in atom table.
Term read_tagged_atom_string(VM &vm, tool::Reader &r) {
  Tag tag = (Tag)r.read_byte();
  switch (tag) {
  case Tag::AtomExt:        return read_atom_string_i16(vm, r);
  case Tag::SmallAtomExt:  return read_atom_string_i8(vm, r);
  default: ;
  }
  throw err::ext_term_error("atom expected");
}


Node *get_node(VM &vm, Term /*sysname*/, dist::creation_t /*creation*/) {
#if FEATURE_ERL_DIST
  G_TODO("distribution support pid etf")
#endif
  return vm.dist_this_node();
}


Term make_pid(VM &vm, Term sysname, word_t id, word_t serial, u8_t creation) {
  if ( !Term::is_valid_pid_id(id)
    || !Term::is_valid_pid_serial(serial)) {
    throw err::ext_term_error("bad pid");
  }
  // TODO: check valid creation
  word_t data = Term::make_pid_data(serial, id);
  auto node = get_node(vm, sysname, creation);

  if (node == vm.dist_this_node()) {
    return Term::make_short_pid(data);
  }
#if FEATURE_ERL_DIST
  G_TODO("distribution support pid etf");
#endif
  // distribution disabled, no want remote pids
  throw err::feature_missing_error("ERL_DIST");
}

Term read_tuple(VM &vm, proc::Heap *heap, tool::Reader &r, word_t arity) {
  if (arity == 0) {
    return Term::make_zero_tuple();
  }

  Term *cells = (Term *)heap->allocate<word_t>(layout::TUPLE::box_size(arity));

  // fill elements or die horribly if something does not decode
  for (word_t i = 0; i < arity; ++i) {
    layout::TUPLE::element(cells, i) = read_ext_term(vm, heap, r);
  }

  return Term::make_tuple(cells, arity);
}


Term read_ext_term_with_marker(VM &vm, proc::Heap *heap, tool::Reader &r) {
  r.assert_byte(etf_marker);
  return read_ext_term(vm, heap, r);
}


#if FEATURE_MAPS
Term read_map(Heap *heap, tool::Reader &r) {
  word_t arity = r.read_bigendian_i32();

  if (arity == 0) {
    return Term::make_zero_map();
  }

  for (auto i = 0; i < arity; ++i) {
    Term key = read_ext_term2(heap, r);
    if (key.is_non_value()) {
      return key;
    }

    Term val = read_ext_term2(heap, r);
    if (val.is_non_value()) {
      return val;
    }
  }
}
#endif // FEATURE_MAPS


Term read_string_ext(proc::Heap *heap, tool::Reader &r) {
  word_t length = r.read_big_u16();
  if (length == 0) {
    return NIL;
  }

  Term result = NIL;
  Term *ref = &result;

  for (word_t i = 0; i < length; ++i) {
    Term *cons = (Term *)heap->allocate<word_t>(layout::CONS::BOX_SIZE);
    layout::CONS::head(cons) = Term::make_small(r.read_byte());
    *ref = Term::make_cons(cons);
    ref = &layout::CONS::tail(cons);
  }

  *ref = NIL;
  return result;
}

Term read_list_ext(VM &vm, proc::Heap *heap, tool::Reader &r) {
  word_t length = r.read_big_u32();

  Term result = NIL;
  Term *ref = &result;

  for (sword_t i = (sword_t)length - 1; i >= 0; i--) {
    Term *cons = (Term *)heap->allocate<word_t>(layout::CONS::BOX_SIZE);

    layout::CONS::head(cons) = read_ext_term(vm, heap, r);
    *ref = Term::make_cons(cons);
    ref = &layout::CONS::tail(cons);
  }

  *ref = read_ext_term(vm, heap, r);
  return result;
}

static Term read_binary(VM &vm, proc::Heap *heap, tool::Reader &r) {
  word_t length = r.read_big_u32();
  Term result = Term::make_binary(vm, heap, length);
  u8_t *data = result.binary_get<u8_t>();
  r.read_bytes(data, length);
  return result;
}

Term read_ext_term(VM &vm, proc::Heap *heap, tool::Reader &r) {
  auto t = (Tag)r.read_byte();
  switch (t) {
  case Tag::Compressed:
    // =80; 4 bytes size; compressed data
    G_TODO("compressed etf");
    G_IF_NODEBUG(break;)

  case Tag::SmallIntegerExt:
    return Term::make_small(r.read_byte());

  case Tag::IntegerExt: {
      // 32-bit integer
      sword_t n = r.read_big_s(4);
      if (get_hardware_bits() > 32) {
        // fits into small_int if platform is x64
        return Term::make_small(n);
      } else { // hardware bits = 32
#if FEATURE_BIGNUM
      if (Term::is_big(n)) {
        G_TODO("construct bignum etf");
      } else {
        return Term::make_small(n);
      }
#else
      // no bignum, and hardware bits not enough: much fail here
      throw err::feature_missing_error("BIGNUM");
#endif
      } // hardware bits = 32
    }  // integer_ext

#if FEATURE_FLOAT
  case OLD_FLOAT_STRING_EXT: {
    G_TODO("parse float string etf");
    } // old string float_ext
  case IEEE_FLOAT_EXT: {
      G_TODO("make ieee 8byte double etf");
    } // new 8byte double float_ext
#else
  case Tag::OldFloatStringExt:
  case Tag::IeeeFloatExt:
    throw err::feature_missing_error("FLOAT");
#endif

  case Tag::AtomUtf8Ext:    // fall through
  case Tag::AtomExt:        return read_atom_string_i16(vm, r);

  case Tag::SmallAtomUtf8Ext: // fall through
  case Tag::SmallAtomExt:     return read_atom_string_i8(vm, r);

  case Tag::ReferenceExt: {
      // format: N atom string, 4byte id, 1byte creation
//      Term node = read_atom_string(r);
//      word_t id = r.read_bigendian_i32();
//      u8_t creation = r.read_byte();
      G_TODO("ref etf");
      G_IF_NODEBUG(break;)
    } // end reference_ext

  case Tag::PortExt: {
      // format: N atom string, 4byte id, 1byte creation
//      Term node = read_atom_string(r);
//      word_t id = r.read_bigendian_i32();
//      u8_t creation = r.read_byte();
      G_TODO("port etf");
      G_IF_NODEBUG(break;)
    } // end reference_ext

  case Tag::PidExt: {
      // format: N atom string, 4byte id, 4byte serial, 1byte cre
      Term node     = read_tagged_atom_string(vm, r);
      word_t id     = r.read_big_u32();
      word_t serial = r.read_big_u32();
      u8_t creation = r.read_byte();
      return make_pid(vm, node, id, serial, creation);
    } // end reference_ext

  case Tag::SmallTupleExt: return read_tuple(vm, heap, r, r.read_byte());
  case Tag::LargeTupleExt: return read_tuple(vm, heap, r, r.read_big_u32());

  case Tag::MapExt:
    if (feature_maps) {
      //return read_map(heap, r);
      throw err::todo_error("etf MAPS");
    } else {
      throw err::feature_missing_error("MAPS");
    }

  case Tag::NilExt:     return NIL;
  case Tag::StringExt:  return read_string_ext(heap, r);
  case Tag::ListExt:    return read_list_ext(vm, heap, r);

  case Tag::BinaryExt:     return read_binary(vm, heap, r);
  case Tag::BitBinaryExt: throw err::todo_error("read bit-binary etf");

  case Tag::SmallBigExt:
  case Tag::LargeBigExt:
    if (feature_bignum) {
      throw err::feature_missing_error("BIGNUM");
    } else {
      throw err::todo_error("BIGNUM");
    }

  default:
    Std::fmt("invalid ETF value tag %d\n", t);
    throw err::ext_term_error("bad etf tag");
  } // switch tag
} // parse function


} // ns etf
} // ns gluon
