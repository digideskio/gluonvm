## THE NEXT <span style="color:silver">~~BIG~~</span> small THING™
# Gluon Erlang Abstract Machine

A configurable and small virtual machine which runs bytecode derived from and 
very similar to Erlang BEAM. The goal is to go smaller than several tens of 
kilobytes (oh well, under megabyte would be cool).

Idea of minimization is to track features used by code automatically plus allow
programmer to specify certain features he'd like to have or not have, then 
compile core VM with these features as executable or firmware. 

## emulator/

C++ implementation of minimalistic BEAM virtual machine. Has simple configurable
feature settings in `include/g_defs.h` (like distribution, float, bignum support etc).

Build with cmake (run `cmake_linux.sh`, then in `_build/` directory run `make`). 
`CMakeLists.txt` work with QtCreator as IDE not to mention cmake supporting host of
other IDEs.

# TODO

* Processes (simple scheduling is done just not connected to the whole thing yet)
* Messages
* Make proper heap and stack
* Simple GC
* Binaries
* Floats maybe?

## apps/asm, apps/emuemu and apps/prototype

Few apps which are currently out of use, but were created to help me learn BEAM universe:
A simple beam-to-something-else assembler, a high level interpreter for GluonVM 
intermediate representation (IR), which simulates properties of Erlang VM (memory,
instruction set, etc) and processes (registers, stack, process execution state).

apps/prototype is based on github.com/tonyrog/beam (owe a beer to Tony!).
