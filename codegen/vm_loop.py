#!/usr/bin/python
# takes: genop.tab from erlang/otp
# produces: g_vm_loop includable file with label copypaste template for vm 
# loop function
import libgenop

libgenop.load()

print("""// Generated by codegen/vm_loop.py

#include "g_vm.h"
#include "g_process.h"

// has own gluon namespace
#include "g_vm_impl.h"

namespace gluon {
""")

print("""void VM::vm_loop(Process *proc) {
  void *jmp_to;
  if (!proc) {
    goto vm_jump_table_init;
  }

next_instr:
  jmp_to = proc->vm_fetch_instr();
  goto *jmp_to;
""")
for opcode in range(libgenop.MIN_OPCODE, libgenop.MAX_OPCODE+1):
    op = libgenop.ops_by_code[opcode]
    if op['name'] == "int_code_end":
        print("""OP_%s: // opcode: %s
  goto vm_end;
""" % (op['name'], opcode))
        continue

    print("OP_%s: // opcode: %d" % (op['name'], opcode))
    if op['name'] in libgenop.implemented_ops:
        print("  impl::opcode_%s(proc);" % (op['name']))
    else:
        print("  G_FAIL(\"notimpl %s\");" % (op['name']))
    print("  G_IF_NODEBUG(goto next_instr;)")
    print

#
# Init table with labels and export it to global scope
#
print("""vm_jump_table_init: {
  // Init table with labels and export it to global scope
  static const void *l_opcode_labels[] = { nullptr,""")

for opcode in range(libgenop.MIN_OPCODE, libgenop.MAX_OPCODE+1):
    op = libgenop.ops_by_code[opcode]
    print("      &&OP_%s," % (op['name']))

print("""    };
    g_opcode_labels = l_opcode_labels;
  } // end init

vm_end: ;
}

} // ns gluon
""")
