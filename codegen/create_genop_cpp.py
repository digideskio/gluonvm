#!/usr/bin/python
# takes: genop.tab from erlang/otp
# produces: g_genop.cpp with arrays that map erlang asm opcodes to numbers
import libgenop

libgenop.load()

print("""// Generated by codegen/vm_genop_cpp.py

#include "defs.h"
#include "genop.h"

namespace gluon {
namespace genop {
""")

# print arity map
print("const unsigned char arity_map[] = {0, // opcode 0 does not exist")
for opcode in range(libgenop.MIN_OPCODE, libgenop.MAX_OPCODE+1):
    op = libgenop.ops_by_code[opcode]
    print("    %d, // opcode: %d (%s)" % (op['arity'], opcode, op['name']))
print("};")
print

# print opcode names map
print("#if G_DEBUG")
print("const char *opcode_name_map[] = {nullptr, // opcode 0 does not exist")
for opcode in range(libgenop.MIN_OPCODE, libgenop.MAX_OPCODE+1):
    op = libgenop.ops_by_code[opcode]
    print("    \"%s\", // opcode: %d" % (op['name'], opcode))
print("};")
print("#endif // G_DEBUG")
print

print("} // ns genop")
print("} // ns gluon")
print
