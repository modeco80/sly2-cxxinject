BINDIR = bin/$(REGION)
OBJDIR = obj/$(REGION)

NAME = sly2_test
TYPE := elf
DEBUG ?= 0

# Runtime sources (startup code, and code
# needed to link the ml libs)
RUNTIME := \
	crt0.S \
	runtime/eekernel.S \
	runtime/mutex.cpp \
	runtime/ml_abort.cpp \
	runtime/simpleheap.cpp \
	runtime/ml_memory.cpp

# General sources. Put all your code here.
# Make sure to put it after $(RUNTIME) please.
SOURCES := \
	$(RUNTIME) \
	main.cpp

LINKSCRIPT = $(OBJDIR)/blob.ld
LINKLIBS := \
	$(PATCHSDK)/src/utils/obj/libutils.a \
	$(PATCHSDK)/src/ml/lib/core/obj/libml3_core.a \
	$(PATCHSDK)/src/ml/lib/cxx/obj/libml3_cxx.a \
	$(PATCHSDK)/src/ml/lib/hw/obj/libml3_hw.a \

# User C/CXX flags.
USER_CFLAGS = -DREGION_$(REGION) \
	-I $(TOP)/sly2_api/include \
	-I $(PATCHSDK)/src/utils/include
USER_CXXFLAGS := \
	-Wno-pmf-conversions

# Pre deps
EXTRATARGETS_PRE = $(LINKSCRIPT) $(OBJDIR)/stub.ld
EXTRATARGETS_POST = \
	$(BINDIR)/$(NAME).bin \
	$(BINDIR)/stub.elf \
	$(TOP)/sly2_test_patch_$(REGION).xdelta

# All files that make up the mkpatch tool.
# Used to make sure that we rebuild the xdelta
# if the Python code is changed.
MKPATCH_DEPS := \
	mkpatch/main.py \
	mkpatch/patches.py \

include $(PATCHSDK)/mk/makedef.mk

# Custom rules start here
# You probably don't need to touch these, if you do,
# PLEASE make sure you know what you're doing!

# Preprocess the linker script to include game symbols here.
$(OBJDIR)/blob.ld: $(OBJDIR)/ blob.ld.in
	$(info Preprocessing linker script $@)
	sed 's|@REGION@|$(REGION)|' blob.ld.in > $@

$(BINDIR)/$(NAME).bin: $(BINDIR)/$(NAME).elf
	$(info Making binary of blob $@)
	$(OBJCOPY) -O binary $< $@

# Rules for building the copy stub

# Makes an object for embedding the blob into the stub
$(OBJDIR)/$(NAME).bin.o: $(BINDIR)/$(NAME).bin
	$(info Copying binary $< to object $@)
	$(OBJCOPY) -I binary -O elf32-littlemips \
		--redefine-sym _binary_$(subst /,_,$(subst .,_,$<))_start=_blob \
		--redefine-sym _binary_$(subst /,_,$(subst .,_,$<))_end=_blob_end \
		--redefine-sym _binary_$(subst /,_,$(subst .,_,$<))_size=_blob_size \
		$< $@
	$(TOP)/tools/objpatch.py $@

$(OBJDIR)/stub.ld: stub/stub.ld.in
	sed 's|@REGION@|$(REGION)|' $< > $@

$(BINDIR)/stub.elf: $(OBJDIR)/$(NAME).bin.o $(OBJDIR)/stub.ld stub/stub.S
	$(info Assembling copy stub $@)
	$(CC) -xassembler-with-cpp -c $(CCFLAGS) stub/stub.S -o $(OBJDIR)/stub.o
	$(LD) -T $(OBJDIR)/stub.ld $(OBJDIR)/stub.o $(OBJDIR)/$(NAME).bin.o -o $@

# Build the final .xdelta which can be distributed (somewhat) legally
$(TOP)/sly2_test_patch_$(REGION).xdelta: $(BINDIR)/stub.elf $(MKPATCH_DEPS)
	$(info Making patched ELF $@)
	mkpatch/main.py $(REGION) $(GAME) $@

