# Exercise

## Exercise 1
Bare-Metal Hello World

This exercise aims to practice low-level programming concepts using x86 assembly language. You'll work with the example programs 'hw-01.asm' or 'hw-02.asm', rewriting them as 'hw.S' using AT&T assembly syntax.

Steps:

1. **Assembler**:  
* Rewrite 'eg.asm' as 'hw.S'.  
* Create an ad hoc assembler+linker program 'hwasm' in C to produce a flat-binary 'hw.bin' suitable for x86 BIOS legacy boot.

2. **Comparison and Testing**:  
* Assemble 'hw.S' using 'hwasm'.  
* Compare 'hw.bin' with 'eg.bin'.  
* Disassemble 'hw.bin' and compare with 'hw.S'.  
* Boot 'hw.bin' using QEMU emulator.  
* Boot 'hw.bin' on real hardware.  

3. **Handling BIOS Issues**:  
* Modify 'hw.S' to handle BIOS assumptions about boot media formatting.  
* Test modifications in the emulator and on real hardware.  

4. **GNU Build Chain Test**:  
* Assemble 'hw.S' with 'as' to produce 'hw2.o'.  
* Link 'hw2.o' with 'ld' to produce 'hw2.bin'.  
* Boot 'hw2.bin' using QEMU.  
* Compare 'hw.bin' and 'hw2.bin'.  
* Optionally, boot 'hw.bin' on real hardware.  

## Exercise 2 
BootCmd: Simple Command-Line Interpreter  

BootCmd is a minimal command-line interpreter fitting into a USB stick's first 512-byte sector. It runs in x86 real-mode via BIOS legacy boot.  

Instructions:  

1. **Build and Execute BootCmd**    
   * Run make followed by make bcmd.bin/run.
     
2. **Understanding the Program**  
  * 'main.c' contains the main program in C.  
  * Assembly functions in 'bios.S' and 'utils.c' for 'strcmp'.  
  * 'bcmd.ld' manages linking, relocation, and boot signature.  
  * 'rt0.o' initializes C runtime.  
  * Functions follow fastcall convention.  
  * See 'Makefile' for build details.  

3 **Implement a New Command**:  
  * function to print the RAM memory storage  
