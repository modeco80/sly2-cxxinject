#include <regdef.h>
/* EE kernel syscall stubs */

.set noat
.set noreorder

.text

.macro genSyscall, name, number
.global \name
.ent \name
.type \name function
\name:
	li v1, \number
	syscall
	jr ra
	nop
.end \name
.endm

genSyscall GetThreadId, 0x2f
genSyscall CreateSema, 0x40
genSyscall DeleteSema, 0x41
genSyscall SignalSema, 0x42
genSyscall WaitSema, 0x44
