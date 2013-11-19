opt= noopt
debug= debug
assert=assert
prof=noprof
trace=notrace

CC=     gcc
DEPCC=  gcc
INCLUDES= -I. $(src_dirs:%=-I%)
OPT=      
DEBUG=
CPPFLAGS= $(INCLUDES)
CFLAGS=   $(WARNINGS) $(OPT) $(DEBUG) $(PROFILE)
COMPILE.c= $(ACC) $(CFLAGS) $(CPPFLAGS) -c
LINK.c=    $(ACC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) 
sun3as=	/usr/local/sun3/bin/sun3-as
sun3ld=	/usr/local/sun3/bin/sun3-ld

src= 	sun3_add_byte.c \
	sun3_add_word.c \
	sun3_add_long.c \
	sun3_adda.c \
	sun3_addi.c \
	sun3_addq_or_subq_byte.c \
	sun3_addq_or_subq_word.c \
	sun3_addq_or_subq_long.c \
	sun3_address_regs.c \
	sun3_address_regs_dump.c \
	sun3_and_byte.c \
	sun3_and_word.c \
	sun3_and_long.c \
	sun3_andi.c \
	sun3_bcc_or_bsr.c \
	sun3_clr.c \
	sun3_cmpal.c \
	sun3_cmpaw.c \
	sun3_cmpb.c \
	sun3_cmpib.c \
	sun3_cmpil.c \
	sun3_cmpiw.c \
	sun3_cmpl.c \
	sun3_cmpw.c \
	sun3_data_regs.c \
	sun3_data_regs_dump.c \
	sun3_dbcc.c \
	sun3_ea.c \
	sun3_ea_mode_rw_byte.c \
	sun3_ea_mode_rw_word.c \
	sun3_ea_mode_rw_long.c \
	sun3_ea_mode_read_byte.c \
	sun3_ea_mode_read_word.c \
	sun3_ea_mode_read_long.c \
	sun3_eor_byte.c \
	sun3_eor_word.c \
	sun3_eor_long.c \
	sun3_eori.c \
	sun3_aout_read.c \
	sun3_error.c \
	sun3_errors.c \
	sun3_memory.c \
	sun3_memory_read.c \
	sun3_memory_write.c \
	sun3_memory_io.c \
	sun3_move_byte.c \
	sun3_move_word.c \
	sun3_move_long.c \
	sun3_movem.c \
	sun3_mul.c \
	sun3_neg_byte.c \
	sun3_neg_word.c \
	sun3_neg_long.c \
	sun3_not_byte.c \
	sun3_not_word.c \
	sun3_not_long.c \
	sun3_or_byte.c \
	sun3_or_word.c \
	sun3_or_long.c \
	sun3_ori.c \
	sun3_scc.c \
	sun3_sub_byte.c \
	sun3_sub_word.c \
	sun3_sub_long.c \
	sun3_suba.c \
	sun3_subi.c \
	sun3_swap.c \
	sun3_trap.c \
	sun3_tstb.c \
	sun3_tstw.c \
	sun3_tstl.c \
	sun3_run.c \
	sun3.c

src_dirs= instr \
	trace/$(trace)

deps=	$(src:%.c=%.d)
obj=	$(src:%.c=%.o)
asm=	$(src:%.c=%.s)

.PHONEY: all

all:	sun3

sun3: $(deps) $(obj) 
	@echo Linking ...
	@$(LINK.c) -o $@ $(obj)

-include $(deps)

ifeq ($(CC),gcc)
  #
  # Machines in 1995 often still had /tmp on a physical disk so -pipe
  # was a must ...
  #
  ACC=$(CC) -pipe  
  #
  # These were great in 1995, no idea if they are still good now.
  #
  WARNINGS= -Wall -W -Wbad-function-cast -Wcast-align -Wcast-qual -Winline \
            -Wmissing-prototypes -Wpointer-arith \
            -Wstrict-prototypes -Wshadow -Wwrite-strings
  ifeq ($(opt),opt)
    OPT =  -O3
  endif
endif

ifeq ($(CC),lcc)
  ACC=$(CC)
  WARNINGS= -A
  ifeq ($(opt),opt)
    OPT=  -O2
  endif
endif

ifeq ($(debug),debug)
  DEBUG = -g
endif

ifeq ($(assert),noassert)
  CFLAGS += -DNDEBUG
endif

ifeq ($(prof),prof)
  PROFILE= -p
else
  ifeq ($(prof),gprof)
    PROFILE = -pg
  endif
endif


%.d: %.c
	@echo Generating dependency data for $< ...
	@$(SHELL) -ec '$(DEPCC) -M $(CPPFLAGS) $< | sed '\''s?^.*:?$*.o $@:?g'\'' > $@'

clean:
	rm -f sun3 $(obj)

realclean: clean
	rm -f $(deps)

distclean:	realclean

sun3bins:	examples/calculator examples/fact examples/forth examples/hanoi examples/ramtest

examples/calculator:	examples/calculator.o examples/libkk.o
	$(sun3ld) -o examples/calculator examples/calculator.o examples/libkk.o

examples/fact:	examples/fact.o examples/libkk.o
	$(sun3ld) -o examples/fact examples/fact.o examples/libkk.o

examples/forth:	examples/forth.o examples/m.o
	$(sun3ld) -N -e init -o examples/forth examples/forth.o examples/m.o

examples/hanoi:	exampes/hanoi.o esamples/libkk.o
	$(sun3ld) -o examples/hanoi examples/hanoi.o examples/libkk.o

examples/ramtest:	examples/ramtest.o
	$(sun3ld) -o examples/ramtest examples/ramtest.o

examples/%.o:	examples/%.s
	$(sun3as) -mc68000 -l -o $@ $<
