#!/bin/env python3

# A very quick and dirty Hack:tm:.
# Binutils 2.10 doesn't have the options to specify architecure, and thus,
# outputs EM_NONE and 0x0 flags when creating a ELF object wrapping binary data.
# We need to patch this to make the link of the copy stub work.

import sys
import struct

OBJ_FILENAME = sys.argv[1]

EM_MIPS = 0x8
EF_R5900FLAGS = 0x20924001

# Read object
with open(OBJ_FILENAME, 'rb') as objFile:
	data = bytearray(objFile.read())

# Make sure it's actually a valid ELF object first, before trying to do anything
if data[0:4] != b'\x7fELF':
	raise RuntimeError('not an ELF object')

# patch e32 header e_mach to EM_MIPS
if struct.unpack("<H", data[0x12:0x14])[0] != EM_MIPS:
	print('Patching e32 e_mach to EM_MIPS')
	data[0x12:0x14] = struct.pack("<H", EM_MIPS)

# patch e32 header e_flags to match R5900
if struct.unpack("<I", data[0x24:0x28])[0] != EF_R5900FLAGS:
	print('Patching e32 e_flags')
	data[0x24:0x28] = struct.pack("<I", 0x20924001)

# Write the patched object
with open(OBJ_FILENAME, 'wb') as objFile:
	objFile.write(data)
