#include <regdef.h>

/**
 * Stub. This is linked relatively high in memory,
 * so it is not overwritten, and fixes up (copies) the blob
 * to the correct location.
 */

.text
.set noreorder

/* Stub start. This is thunked to StartupMemMgr(), which the blob takes over
 to initalize everything. */
.global _start
_start:
	la t0, BLOB_LOAD_ADDRESS
	la t1, _blob
	la t2, _blob_size
	/* Fix the blob up to the correct location.
	 This overwrites the IRX data part of the heap (StartupSif is done with it at this point)
	 and replaces it with the blob. */
.Lcopy:
	lbu t3, (t1)
	sb t3, (t0)
	addi t0, t0, 1
	addi t1, t1, 1
	addi t2, t2, -1
	bne t2, zero, .Lcopy
	nop
.Ldone:
	/* Jump to the start of the blob. It will do the rest for us.
	   Note that this assumes _start is the first thing in the blob, which will be true. */
	j BLOB_LOAD_ADDRESS
	nop
