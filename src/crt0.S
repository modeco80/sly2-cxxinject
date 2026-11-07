#include <regdef.h>

#define at $1

/*
 * Minimal startup code for mod code.
 *
 * Where we are jumped into, we have a lot of stuff which has been pre-initialized for us
 * so we do not really need to do too  much of anything.
 */

.set noat
.set noreorder

.text

.global _start
_start:

	/* Zero-fill our .bss section. */
	la t0, _fbss
	la t2, _bss_size
.Lwipebss:
	/* These two nops correct for EE errata with branch instructions.
	  PCSX2 does not emulate EE errata but it's still a good idea. */
	nop
	nop
	sb zero, (t0)
	addi t0, t0, 1
	addi t1, t1, -1
	bne t1, zero, .Lwipebss
	nop

	/* Save registers which are clobbered on the stack. */
	addiu sp, sp, -0x8
	sw ra, (sp)
	sw gp, 4(sp)

	/* Do Sly heap init. */
	jal slyHeapInit
	nop

	/* Call C++ global constructors. */
	jal __do_global_ctors
	nop

	/* Now that .bss should hopefully be filled, and
	 we've called global constructors, call the C++ entry point. */
	jal modMain
	nop

	/* Check the return value of modMain().
	  IF it is > 0, something went wrong and we execute global destructors. */
	slti at, v0, 0x0
	beq at, zero, .Lokay
	nop

	jal __do_global_dtors
	nop

.Lokay:
	/* Restore registers which were clobbered from the stack
	 and clean up the stack, preparing to return back to the game */
	lw gp, 4(sp)
	lw ra, (sp)

	jr ra
	addiu sp, sp, 0x8

