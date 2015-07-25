#!/usr/bin/python
# takes: genop.tab from erlang/otp
# produces: g_genop.h header with arrays that map erlang asm opcodes to numbers
import libgenop

libgenop.load()

print("// Generated by codegen/vm_genop_h.py")
print("#pragma once")

print("namespace gluon {")
print("namespace genop {")

print

print("const unsigned int MIN_OPCODE = %d;" % libgenop.MIN_OPCODE)
print("const unsigned int MAX_OPCODE = %d;" % libgenop.MAX_OPCODE)
print

# print constants
for opcode in range(libgenop.MIN_OPCODE, libgenop.MAX_OPCODE+1):
    op = libgenop.ops_by_code[opcode]
    print("const u8_t OPCODE_%s = %d; // 0x%x" % (op['name'].upper(), opcode, opcode));
print

# print arity map
print("extern const unsigned char arity_map[];")
print

# print opcode names map
print("#if G_DEBUG")
print("extern const char *opcode_name_map[];")
print("#endif // G_DEBUG")
print

print("} // ns genop")
print("} // ns gluon")
print