# Boot, say hello, and halt
# AT&T assembly syntax, naive char by char, manually

	.data		# Data section
	.globl _start	# Global entry point

_start:
	movb $0xe, %ah	# Set BIOS teletype mode

	movb $'H', %al	# Load 'H' ascii code
	int $0x10	# Issue BIOS interrupt

	movb $'e', %al	# Load 'e' ascii code
	int $0x10	# Issue BIOS interrupt

	movb $'l', %al	# Load 'l' ascii code
	int $0x10	# Issue BIOS interrupt

	movb $'l', %al	# Load 'l' ascii code
	int $0x10	# Issue BIOS interrupt

	movb $'o', %al	# Load 'o' ascii code
	int $0x10	# Issue BIOS interrupt

	movb $' ', %al	# Load ' ' ascii code
	int $0x10	# Issue BIOS interrupt

	movb $'W', %al	# Load 'W' ascii code
	int $0x10	# Issue BIOS interrupt

	movb $'o', %al	# Load 'o' ascii code
	int $0x10	# Issue BIOS interrupt

	movb $'r', %al	# Load 'r' ascii code
	int $0x10	# Issue BIOS interrupt

	movb $'l', %al	# Load 'l' ascii code
	int $0x10	# Issue BIOS interrupt

	movb $'d', %al	# Load 'd' ascii code
	int $0x10	# Issue BIOS interrupt

halt:	
	hlt		# Halt the machine
	jmp halt	# Safeguard 

	.balign 510, 0	# Pad with zeros
	.word 0xaa55	# Boot signature
