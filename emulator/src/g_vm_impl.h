#pragma once
// Generated by codegen/vm_copypaste_impl.py

#include "g_process.h"

namespace gluon {
namespace impl {

inline void opcode_move(Process *proc) { // opcode: 64
  Term arg1 = proc->vm_fetch_term();
  Term arg2 = proc->vm_fetch_term();
  if (arg2.is_regx()) {
    proc->set_x(arg2.regx_get_value(), arg1);
  } else {
    G_FAIL("bad move dst")
  }
}

//  inline void opcode_label(Process *proc) { // opcode: 1
//  }
//  inline void opcode_func_info(Process *proc) { // opcode: 2
//  }
//  inline void opcode_int_code_end(Process *proc) { // opcode: 3
//  }
//  inline void opcode_call(Process *proc) { // opcode: 4
//  }
//  inline void opcode_call_last(Process *proc) { // opcode: 5
//  }
//  inline void opcode_call_only(Process *proc) { // opcode: 6
//  }
//  inline void opcode_call_ext(Process *proc) { // opcode: 7
//  }
//  inline void opcode_call_ext_last(Process *proc) { // opcode: 8
//  }
//  inline void opcode_bif0(Process *proc) { // opcode: 9
//  }
//  inline void opcode_bif1(Process *proc) { // opcode: 10
//  }
//  inline void opcode_bif2(Process *proc) { // opcode: 11
//  }
//  inline void opcode_allocate(Process *proc) { // opcode: 12
//  }
//  inline void opcode_allocate_heap(Process *proc) { // opcode: 13
//  }
//  inline void opcode_allocate_zero(Process *proc) { // opcode: 14
//  }
//  inline void opcode_allocate_heap_zero(Process *proc) { // opcode: 15
//  }
//  inline void opcode_test_heap(Process *proc) { // opcode: 16
//  }
//  inline void opcode_init(Process *proc) { // opcode: 17
//  }
//  inline void opcode_deallocate(Process *proc) { // opcode: 18
//  }
//  inline void opcode_return(Process *proc) { // opcode: 19
//  }
//  inline void opcode_send(Process *proc) { // opcode: 20
//  }
//  inline void opcode_remove_message(Process *proc) { // opcode: 21
//  }
//  inline void opcode_timeout(Process *proc) { // opcode: 22
//  }
//  inline void opcode_loop_rec(Process *proc) { // opcode: 23
//  }
//  inline void opcode_loop_rec_end(Process *proc) { // opcode: 24
//  }
//  inline void opcode_wait(Process *proc) { // opcode: 25
//  }
//  inline void opcode_wait_timeout(Process *proc) { // opcode: 26
//  }
//  inline void opcode_m_plus(Process *proc) { // opcode: 27
//  }
//  inline void opcode_m_minus(Process *proc) { // opcode: 28
//  }
//  inline void opcode_m_times(Process *proc) { // opcode: 29
//  }
//  inline void opcode_m_div(Process *proc) { // opcode: 30
//  }
//  inline void opcode_int_div(Process *proc) { // opcode: 31
//  }
//  inline void opcode_int_rem(Process *proc) { // opcode: 32
//  }
//  inline void opcode_int_band(Process *proc) { // opcode: 33
//  }
//  inline void opcode_int_bor(Process *proc) { // opcode: 34
//  }
//  inline void opcode_int_bxor(Process *proc) { // opcode: 35
//  }
//  inline void opcode_int_bsl(Process *proc) { // opcode: 36
//  }
//  inline void opcode_int_bsr(Process *proc) { // opcode: 37
//  }
//  inline void opcode_int_bnot(Process *proc) { // opcode: 38
//  }
//  inline void opcode_is_lt(Process *proc) { // opcode: 39
//  }
//  inline void opcode_is_ge(Process *proc) { // opcode: 40
//  }
//  inline void opcode_is_eq(Process *proc) { // opcode: 41
//  }
//  inline void opcode_is_ne(Process *proc) { // opcode: 42
//  }
//  inline void opcode_is_eq_exact(Process *proc) { // opcode: 43
//  }
//  inline void opcode_is_ne_exact(Process *proc) { // opcode: 44
//  }
//  inline void opcode_is_integer(Process *proc) { // opcode: 45
//  }
//  inline void opcode_is_float(Process *proc) { // opcode: 46
//  }
//  inline void opcode_is_number(Process *proc) { // opcode: 47
//  }
//  inline void opcode_is_atom(Process *proc) { // opcode: 48
//  }
//  inline void opcode_is_pid(Process *proc) { // opcode: 49
//  }
//  inline void opcode_is_reference(Process *proc) { // opcode: 50
//  }
//  inline void opcode_is_port(Process *proc) { // opcode: 51
//  }
//  inline void opcode_is_nil(Process *proc) { // opcode: 52
//  }
//  inline void opcode_is_binary(Process *proc) { // opcode: 53
//  }
//  inline void opcode_is_constant(Process *proc) { // opcode: 54
//  }
//  inline void opcode_is_list(Process *proc) { // opcode: 55
//  }
//  inline void opcode_is_nonempty_list(Process *proc) { // opcode: 56
//  }
//  inline void opcode_is_tuple(Process *proc) { // opcode: 57
//  }
//  inline void opcode_test_arity(Process *proc) { // opcode: 58
//  }
//  inline void opcode_select_val(Process *proc) { // opcode: 59
//  }
//  inline void opcode_select_tuple_arity(Process *proc) { // opcode: 60
//  }
//  inline void opcode_jump(Process *proc) { // opcode: 61
//  }
//  inline void opcode_catch(Process *proc) { // opcode: 62
//  }
//  inline void opcode_catch_end(Process *proc) { // opcode: 63
//  }
//  inline void opcode_get_list(Process *proc) { // opcode: 65
//  }
//  inline void opcode_get_tuple_element(Process *proc) { // opcode: 66
//  }
//  inline void opcode_set_tuple_element(Process *proc) { // opcode: 67
//  }
//  inline void opcode_put_string(Process *proc) { // opcode: 68
//  }
//  inline void opcode_put_list(Process *proc) { // opcode: 69
//  }
//  inline void opcode_put_tuple(Process *proc) { // opcode: 70
//  }
//  inline void opcode_put(Process *proc) { // opcode: 71
//  }
//  inline void opcode_badmatch(Process *proc) { // opcode: 72
//  }
//  inline void opcode_if_end(Process *proc) { // opcode: 73
//  }
//  inline void opcode_case_end(Process *proc) { // opcode: 74
//  }
//  inline void opcode_call_fun(Process *proc) { // opcode: 75
//  }
//  inline void opcode_make_fun(Process *proc) { // opcode: 76
//  }
//  inline void opcode_is_function(Process *proc) { // opcode: 77
//  }
//  inline void opcode_call_ext_only(Process *proc) { // opcode: 78
//  }
//  inline void opcode_bs_start_match(Process *proc) { // opcode: 79
//  }
//  inline void opcode_bs_get_integer(Process *proc) { // opcode: 80
//  }
//  inline void opcode_bs_get_float(Process *proc) { // opcode: 81
//  }
//  inline void opcode_bs_get_binary(Process *proc) { // opcode: 82
//  }
//  inline void opcode_bs_skip_bits(Process *proc) { // opcode: 83
//  }
//  inline void opcode_bs_test_tail(Process *proc) { // opcode: 84
//  }
//  inline void opcode_bs_save(Process *proc) { // opcode: 85
//  }
//  inline void opcode_bs_restore(Process *proc) { // opcode: 86
//  }
//  inline void opcode_bs_init(Process *proc) { // opcode: 87
//  }
//  inline void opcode_bs_final(Process *proc) { // opcode: 88
//  }
//  inline void opcode_bs_put_integer(Process *proc) { // opcode: 89
//  }
//  inline void opcode_bs_put_binary(Process *proc) { // opcode: 90
//  }
//  inline void opcode_bs_put_float(Process *proc) { // opcode: 91
//  }
//  inline void opcode_bs_put_string(Process *proc) { // opcode: 92
//  }
//  inline void opcode_bs_need_buf(Process *proc) { // opcode: 93
//  }
//  inline void opcode_fclearerror(Process *proc) { // opcode: 94
//  }
//  inline void opcode_fcheckerror(Process *proc) { // opcode: 95
//  }
//  inline void opcode_fmove(Process *proc) { // opcode: 96
//  }
//  inline void opcode_fconv(Process *proc) { // opcode: 97
//  }
//  inline void opcode_fadd(Process *proc) { // opcode: 98
//  }
//  inline void opcode_fsub(Process *proc) { // opcode: 99
//  }
//  inline void opcode_fmul(Process *proc) { // opcode: 100
//  }
//  inline void opcode_fdiv(Process *proc) { // opcode: 101
//  }
//  inline void opcode_fnegate(Process *proc) { // opcode: 102
//  }
//  inline void opcode_make_fun2(Process *proc) { // opcode: 103
//  }
//  inline void opcode_try(Process *proc) { // opcode: 104
//  }
//  inline void opcode_try_end(Process *proc) { // opcode: 105
//  }
//  inline void opcode_try_case(Process *proc) { // opcode: 106
//  }
//  inline void opcode_try_case_end(Process *proc) { // opcode: 107
//  }
//  inline void opcode_raise(Process *proc) { // opcode: 108
//  }
//  inline void opcode_bs_init2(Process *proc) { // opcode: 109
//  }
//  inline void opcode_bs_bits_to_bytes(Process *proc) { // opcode: 110
//  }
//  inline void opcode_bs_add(Process *proc) { // opcode: 111
//  }
//  inline void opcode_apply(Process *proc) { // opcode: 112
//  }
//  inline void opcode_apply_last(Process *proc) { // opcode: 113
//  }
//  inline void opcode_is_boolean(Process *proc) { // opcode: 114
//  }
//  inline void opcode_is_function2(Process *proc) { // opcode: 115
//  }
//  inline void opcode_bs_start_match2(Process *proc) { // opcode: 116
//  }
//  inline void opcode_bs_get_integer2(Process *proc) { // opcode: 117
//  }
//  inline void opcode_bs_get_float2(Process *proc) { // opcode: 118
//  }
//  inline void opcode_bs_get_binary2(Process *proc) { // opcode: 119
//  }
//  inline void opcode_bs_skip_bits2(Process *proc) { // opcode: 120
//  }
//  inline void opcode_bs_test_tail2(Process *proc) { // opcode: 121
//  }
//  inline void opcode_bs_save2(Process *proc) { // opcode: 122
//  }
//  inline void opcode_bs_restore2(Process *proc) { // opcode: 123
//  }
//  inline void opcode_gc_bif1(Process *proc) { // opcode: 124
//  }
//  inline void opcode_gc_bif2(Process *proc) { // opcode: 125
//  }
//  inline void opcode_bs_final2(Process *proc) { // opcode: 126
//  }
//  inline void opcode_bs_bits_to_bytes2(Process *proc) { // opcode: 127
//  }
//  inline void opcode_put_literal(Process *proc) { // opcode: 128
//  }
//  inline void opcode_is_bitstr(Process *proc) { // opcode: 129
//  }
//  inline void opcode_bs_context_to_binary(Process *proc) { // opcode: 130
//  }
//  inline void opcode_bs_test_unit(Process *proc) { // opcode: 131
//  }
//  inline void opcode_bs_match_string(Process *proc) { // opcode: 132
//  }
//  inline void opcode_bs_init_writable(Process *proc) { // opcode: 133
//  }
//  inline void opcode_bs_append(Process *proc) { // opcode: 134
//  }
//  inline void opcode_bs_private_append(Process *proc) { // opcode: 135
//  }
//  inline void opcode_trim(Process *proc) { // opcode: 136
//  }
//  inline void opcode_bs_init_bits(Process *proc) { // opcode: 137
//  }
//  inline void opcode_bs_get_utf8(Process *proc) { // opcode: 138
//  }
//  inline void opcode_bs_skip_utf8(Process *proc) { // opcode: 139
//  }
//  inline void opcode_bs_get_utf16(Process *proc) { // opcode: 140
//  }
//  inline void opcode_bs_skip_utf16(Process *proc) { // opcode: 141
//  }
//  inline void opcode_bs_get_utf32(Process *proc) { // opcode: 142
//  }
//  inline void opcode_bs_skip_utf32(Process *proc) { // opcode: 143
//  }
//  inline void opcode_bs_utf8_size(Process *proc) { // opcode: 144
//  }
//  inline void opcode_bs_put_utf8(Process *proc) { // opcode: 145
//  }
//  inline void opcode_bs_utf16_size(Process *proc) { // opcode: 146
//  }
//  inline void opcode_bs_put_utf16(Process *proc) { // opcode: 147
//  }
//  inline void opcode_bs_put_utf32(Process *proc) { // opcode: 148
//  }
//  inline void opcode_on_load(Process *proc) { // opcode: 149
//  }
//  inline void opcode_recv_mark(Process *proc) { // opcode: 150
//  }
//  inline void opcode_recv_set(Process *proc) { // opcode: 151
//  }
//  inline void opcode_gc_bif3(Process *proc) { // opcode: 152
//  }
//  inline void opcode_line(Process *proc) { // opcode: 153
//  }
//  inline void opcode_put_map_assoc(Process *proc) { // opcode: 154
//  }
//  inline void opcode_put_map_exact(Process *proc) { // opcode: 155
//  }
//  inline void opcode_is_map(Process *proc) { // opcode: 156
//  }
//  inline void opcode_has_map_fields(Process *proc) { // opcode: 157
//  }
//  inline void opcode_get_map_elements(Process *proc) { // opcode: 158
//  }

} // ns impl
} // ns gluon

