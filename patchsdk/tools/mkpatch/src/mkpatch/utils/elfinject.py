import struct

ELF32_EHDR_FMT = "<16sHHIIIIIHHHHHH"
ELF32_PHDR_FMT = "<IIIIIIII"

ELF32_EHDR_SIZE = struct.calcsize(ELF32_EHDR_FMT)
ELF32_PHDR_SIZE = struct.calcsize(ELF32_PHDR_FMT)

# TODO use these.
ELF_HEADER = struct.Struct("<16sHHIIIIIHHHHHH")
PHDR = struct.Struct("<IIIIIIII")
SHDR = struct.Struct("<IIIIIIIIII")

SHT_NOBITS = 8

PT_LOAD = 1

PF_X = 1
PF_W = 2
PF_R = 4

__all__ = ['PatchInjector']

def elf32SanityChecks(ehdr):
	e_ident = ehdr[0]
	e_phoff = ehdr[5]
	e_phentsize = ehdr[9]
	e_phnum = ehdr[10]

	# Sanity checks.
	if e_ident[:4] != b"\x7fELF":
		raise ValueError("stupidass")

	if e_ident[4] != 1:
		raise ValueError("Not an ELF32. What are we doing here")

	if e_ident[5] != 1:
		raise ValueError("ok you're just being silly on purpose now")

	if e_phentsize != ELF32_PHDR_SIZE:
		raise ValueError(
			f"Incorrect program-header size of {e_phentsize:#x} in ELF file"
		)

def elfAppendProgramHeader(dataIn: bytes, new_phdr):
	data = bytearray(dataIn)
	if len(data) < ELF32_EHDR_SIZE:
		raise ValueError("buffer is physicisally incapable of being an ELF")

	hdr = ELF_HEADER.unpack_from(data)
	ident = hdr[0]
	elf32SanityChecks(hdr)

	e_phoff = hdr[5]
	e_shoff = hdr[6]
	e_ehsize = hdr[8]
	e_phentsize = hdr[9]
	e_phnum = hdr[10]
	e_shentsize = hdr[11]
	e_shnum = hdr[12]

	insertion = e_phoff + e_phnum * e_phentsize
	if insertion > len(data):
		raise ValueError("program header table extends beyond end of ELF")
	if insertion < ELF_HEADER.size:
		raise ValueError("program header can't somehow be before Elf32 header")

	phdrs = [
		list(PHDR.unpack_from(
			data,
			e_phoff + i * e_phentsize
		))
		for i in range(e_phnum)
	]

	shdrs = []

	# Read section headers if we have them
	if e_shoff and e_shnum:
		if e_shoff + e_shnum * e_shentsize > len(data):
			raise ValueError("section header table extends beyond end of ELF")

		shdrs = [
			list(SHDR.unpack_from(
				data,
				e_shoff + i * e_shentsize
			))
			for i in range(e_shnum)
		]

	# Make sure program headers are good
	for i, ph in enumerate(phdrs):
		p_offset = ph[1]
		p_filesz = ph[4]

		if p_filesz and p_offset < insertion < p_offset + p_filesz:
			raise ValueError(f"program header {i} somehow contains insertion point")

	# Make sure that section headers are good
	for i, sh in enumerate(shdrs):
		sh_type = sh[1]
		sh_offset = sh[4]
		sh_size = sh[5]

		if sh_type == SHT_NOBITS or sh_size == 0:
			continue

		if sh_offset < insertion < sh_offset + sh_size:
			raise ValueError(f"section header {i} somehow contains insertion point")

	data[insertion:insertion] = b"\x00" * PHDR.size

	# Bump program header p_offset
	for i, ph in enumerate(phdrs):
		if ph[1] >= insertion:
			ph[1] += PHDR.size
		PHDR.pack_into(
			data,
			e_phoff + i * e_phentsize,
			*ph
		)

	# Bump section headers
	if e_shoff:
		if e_shoff >= insertion:
			e_shoff += PHDR.size

		for i, sh in enumerate(shdrs):
			sh_type = sh[1]
			sh_offset = sh[4]

			if sh_type != SHT_NOBITS and sh_offset >= insertion:
				sh[4] += PHDR.size

			SHDR.pack_into(
				data,
				e_shoff + i * e_shentsize,
				*sh
			)

	# Inject requested program header
	PHDR.pack_into(
		data,
		insertion,
		*new_phdr
	)

	# Update Elf32 header to reflect changes.
	struct.pack_into("<I", data, 32, e_shoff)
	struct.pack_into("<H", data, 44, e_phnum + 1)
	return data


class PatchInjector:
	## Injects by creating a new PHDR in free space.
	@staticmethod
	def injectCreateNewPHDR(original: bytes, patchCore: bytes, patchVirtualLoad: int) -> bytes:
		# Parse the ELF header.
		ehdr = list(struct.unpack_from(ELF32_EHDR_FMT, original, 0))

		elf32SanityChecks(ehdr)

		e_ident = ehdr[0]
		e_phoff = ehdr[5]
		e_phentsize = ehdr[9]
		e_phnum = ehdr[10]


		old_phdr_end = e_phoff + e_phnum * e_phentsize
		new_phdr_end = e_phoff + (e_phnum + 1) * e_phentsize

		# Find the first PT_LOAD PHDR.
		first_load_offset = None
		for i in range(e_phnum):
			phoff = e_phoff + i * e_phentsize

			ph = struct.unpack_from(
				ELF32_PHDR_FMT,
				original,
				phoff,
			)

			p_type = ph[0]
			p_offset = ph[1]

			if p_type == PT_LOAD:
				first_load_offset = p_offset
				break

		# Make sure there is actually enough space to expand the PHDR table
		# in-place, and give up early if there is not.
		if new_phdr_end > first_load_offset:
			raise ValueError(
				"Cannot expand the PHDR table in-place: "
				f"Need through {new_phdr_end:#x}, "
				f"and the first PT_LOAD begins at {first_load_offset:#x}"
			)

		align = 0x1000
		vaddr_offset = patchVirtualLoad % align

		blob_offset = len(original)

		# Round the file end up while preserving the low bits of the virtual address.
		if blob_offset % align <= vaddr_offset:
			blob_offset += vaddr_offset - (blob_offset % align)
		else:
			blob_offset += align + vaddr_offset - (blob_offset % align)

		# Create the new Elf32_Phdr
		patch_phdr = struct.pack(
			ELF32_PHDR_FMT,
			PT_LOAD,					# p_type
			blob_offset,				# p_offset
			patchVirtualLoad,			# p_vaddr
			patchVirtualLoad,			# p_paddr
			len(patchCore),				# p_filesz
			len(patchCore),				# p_memsz
			PF_R | PF_W | PF_X,			# p_flags
			align,						# p_align
		)

		result = bytearray(original)
		struct.pack_into("<H", result, 0x2c, e_phnum + 1)

		# Write the additional PHDR immediately after the existing PHDR.
		result[old_phdr_end:new_phdr_end] = patch_phdr

		if len(result) < blob_offset:
			result.extend(b"\x00" * (blob_offset - len(result)))

		# Append the actual code/data.
		result.extend(patchCore)
		return bytes(result)

	@staticmethod
	def injectAppendNewPHDR(original: bytes, patchCore: bytes, patchVirtualLoad: int) -> bytes:
		align = 0x1000
		blob_offset = len(original)
		vaddr_offset = patchVirtualLoad % align

		# Round the file end up while preserving the low bits of the virtual address.
		if blob_offset % align <= vaddr_offset:
			blob_offset += vaddr_offset - (blob_offset % align)
		else:
			blob_offset += align + vaddr_offset - (blob_offset % align)

		# Append new program header.
		result = elfAppendProgramHeader(original, (
			PT_LOAD,					# p_type
			blob_offset,				# p_offset
			patchVirtualLoad,			# p_vaddr
			patchVirtualLoad,			# p_paddr
			len(patchCore),				# p_filesz
			len(patchCore),				# p_memsz
			PF_R | PF_W | PF_X,			# p_flags
			align,						# p_align
		))

		if len(result) < blob_offset:
			result.extend(b"\x00" * (blob_offset - len(result)))

		# Append the actual code/data.
		result.extend(patchCore)
		return bytes(result)

	### Injects core by overwriting a junk secondary PT_LOAD program header. Used for Metrowerks linker
	### binaries, since they do not give enough space to make a new program header.
	@staticmethod
	def injectOverwriteLastPHDR(original: bytes, patchCore: bytes, patchVirtualLoad: int) -> bytes:
		# Parse the ELF header.
		ehdr = list(struct.unpack_from(ELF32_EHDR_FMT, original, 0))

		elf32SanityChecks(ehdr)

		e_ident = ehdr[0]
		e_phoff = ehdr[5]
		e_phentsize = ehdr[9]
		e_phnum = ehdr[10]

		last_phdr_start = e_phoff + (e_phnum-1) * e_phentsize

		# Make sure the last phdr is a PT_LOAD
		ph = struct.unpack_from(
			ELF32_PHDR_FMT,
			original,
			last_phdr_start,
		)
		assert ph[0] == PT_LOAD
		# TODO: We may want to check if the phdr is actually loading anything,
		# and fail if that happens to be the case.

		align = 0x1000
		vaddr_offset = patchVirtualLoad % align

		blob_offset = len(original)

		# Round the file end up while preserving the low bits of the virtual address.
		if blob_offset % align <= vaddr_offset:
			blob_offset += vaddr_offset - (blob_offset % align)
		else:
			blob_offset += align + vaddr_offset - (blob_offset % align)

		result = bytearray(original)

		# Inject the patch phdr in place of the last phdr.
		patch_phdr = struct.pack_into(
			ELF32_PHDR_FMT,
			result,
			last_phdr_start,
			PT_LOAD,					# p_type
			blob_offset,				# p_offset
			patchVirtualLoad,			# p_vaddr
			patchVirtualLoad,			# p_paddr
			len(patchCore),				# p_filesz
			len(patchCore),				# p_memsz
			PF_R | PF_W | PF_X,			# p_flags
			align,						# p_align
		)

		if len(result) < blob_offset:
			result.extend(b"\x00" * (blob_offset - len(result)))

		# Append the actual code/data.
		result.extend(patchCore)
		return bytes(result)
