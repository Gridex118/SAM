# Simple Abstract machine
## About
SAM is a 16-bit Virtual Machine written in C.  
The memory of the machine has been divided into separate sections for data and code.  
Additionally, the machine makes use of 9 registers([registers.txt](./registers.txt)).  
For the instruction set, which was based on the LC-3 instruction set, please refer to [instr_set.txt](./instr_set.txt).  
SASM(SAM Assembler) is an 'assembler' for SAM written in C++.

## Building
`git clone https://github.com/Gridex118/SAM sam`  
`cd sam`  
`mkdir build`  
`cmake build .`  
if you have multiple compilers installed, use  
`cmake build . -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++`,  
replacing gcc and g++ with your preferred compiler  
`make`  

## Usage
Sasm files are supposed to be assembled to sam files using sasm.out 
`./sasm.out SOURCE.sasm [BIN.sam]`. A few example sasm files are provided
in the examples directory.  
Sam files are named a.sam by default.  
Run the sam files using `./sam.out BIN.sam`  

## Example
While in the build directory,  
`./sasm.out ../examples/ifel.sasm ifel.sam`  
`./sam.out ifel.sam`
