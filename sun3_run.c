#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "sun3_error.h"
#include "sun3_errors.h"
#include "sun3_memory.h"
#include "sun3_memory_read.h"
#include "sun3_memory_write.h"
#include "sun3_data_regs.h"
#include "sun3_address_regs.h"
#include "sun3_sr.h"
#include "sun3.h"
#include "sun3_run.h"
#include "sun3_instr.h"
#include "sun3_trace.h"
#include "sun3_instruction.h"
#include "sun3_ea.h"
#include "sun3_ea_mode_rw_byte.h"
#include "sun3_ea_mode_rw_word.h"
#include "sun3_ea_mode_rw_long.h"
#include "sun3_add_byte.h"
#include "sun3_add_word.h"
#include "sun3_add_long.h"
#include "sun3_adda.h"
#include "sun3_addi.h"
#include "sun3_addq_or_subq_byte.h"
#include "sun3_addq_or_subq_word.h"
#include "sun3_addq_or_subq_long.h"
#include "sun3_and_byte.h"
#include "sun3_and_word.h"
#include "sun3_and_long.h"
#include "sun3_andi.h"
#include "sun3_bcc_or_bsr.h"
#include "sun3_clr.h"
#include "sun3_cmp.h"
#include "sun3_cmpa.h"
#include "sun3_cmpi.h"
#include "sun3_dbcc.h"
#include "sun3_eor_byte.h"
#include "sun3_eor_word.h"
#include "sun3_eor_long.h"
#include "sun3_eori.h"
#include "sun3_jsr.h"
#include "sun3_move_byte.h"
#include "sun3_move_word.h"
#include "sun3_move_long.h"
#include "sun3_movem.h"
#include "sun3_mul.h"
#include "sun3_neg_byte.h"
#include "sun3_neg_word.h"
#include "sun3_neg_long.h"
#include "sun3_not_byte.h"
#include "sun3_not_word.h"
#include "sun3_not_long.h"
#include "sun3_ori.h"
#include "sun3_or_byte.h"
#include "sun3_or_word.h"
#include "sun3_or_long.h"
#include "sun3_scc.h"
#include "sun3_sub_byte.h"
#include "sun3_sub_word.h"
#include "sun3_sub_long.h"
#include "sun3_suba.h"
#include "sun3_subi.h"
#include "sun3_swap.h"
#include "sun3_trap.h"
#include "sun3_tst.h"


int sun3_run(sun3 * vm)
{
	sun3_memory_address pc;
	sun3_unsigned_word pci;
	unsigned int reg_num, ea_mode, size;
	sun3_memory   * mem= &vm->memory;
	sun3_data_regs * data_regs= &vm->data_regs;
	sun3_address_regs * address_regs= &vm->address_regs;
	pc= vm->pc= vm->start_address;
	sun3_address_regs_write_unsigned_long(address_regs, 7, vm->memory.end);
	for (;;) {
		vm->pci = pci = sun3_memory_read_unsigned_word(mem, vm->pc);
		sun3_trace_pc(vm, vm->pc, pci);
		pc = vm->pc += sun3_instrs(1);
		vm->reg_num = reg_num= pci&0x7;
		vm->ea_mode = ea_mode= sun3_instr_ea_mode(pci);
		vm->size = size= sun3_instr_size(pci);
		switch (sun3_instr_opcode(pci)) {
		case 0x0:
			switch (sun3_instr_subcode(pci)) {
			case sun3_instr_ORI:
				switch (size) {
				case sun3_instr_size_BYTE:
					sun3_instr_ori_byte.src= sun3_memory_read_unsigned_word(&vm->memory, pc);
					vm->pc += sizeof(sun3_unsigned_word);
					sun3_ea_mode_rw_byte(vm, &sun3_instr_ori_byte);
					break;
				case sun3_instr_size_LONG:
					sun3_instr_ori_long.src= sun3_memory_read_unsigned_long(&vm->memory, pc);
					vm->pc += sizeof(sun3_unsigned_long);
					sun3_ea_mode_rw_long(vm, &sun3_instr_ori_long);
					break;
				case sun3_instr_size_WORD:
					sun3_instr_ori_word.src= sun3_memory_read_unsigned_word(&vm->memory, pc);
					vm->pc += sizeof(sun3_unsigned_word);
					sun3_ea_mode_rw_word(vm, &sun3_instr_ori_word);
					break;
				default:
					sun3_errors_illegal_instruction(vm->errors);
				}
				break;
			case sun3_instr_ANDI:
				switch (size) {
				case sun3_instr_size_BYTE:
					sun3_instr_andi_byte.src= sun3_memory_read_unsigned_word(&vm->memory, pc);
					vm->pc += sizeof(sun3_unsigned_word);
					sun3_ea_mode_rw_byte(vm, &sun3_instr_andi_byte);
					break;
				case sun3_instr_size_LONG:
					sun3_instr_andi_long.src= sun3_memory_read_unsigned_long(&vm->memory, pc);
					vm->pc += sizeof(sun3_unsigned_long);
					sun3_ea_mode_rw_long(vm, &sun3_instr_andi_long);
					break;
				case sun3_instr_size_WORD:
					sun3_instr_andi_word.src= sun3_memory_read_unsigned_word(&vm->memory, pc);
					vm->pc += sizeof(sun3_unsigned_word);
					sun3_ea_mode_rw_word(vm, &sun3_instr_andi_word);
					break;
				default:
					sun3_errors_illegal_instruction(vm->errors);
				}
				break;
			case sun3_instr_SUBI:
				switch (size) {
				case sun3_instr_size_BYTE:
					sun3_instr_subi_byte.src= sun3_memory_read_unsigned_word(&vm->memory, pc);
					vm->pc += sizeof(sun3_unsigned_word);
					sun3_ea_mode_rw_byte(vm, &sun3_instr_subi_byte);
					break;
				case sun3_instr_size_LONG:
					sun3_instr_subi_long.src= sun3_memory_read_unsigned_long(&vm->memory, pc);
					vm->pc += sizeof(sun3_unsigned_long);
					sun3_ea_mode_rw_long(vm, &sun3_instr_subi_long);
					break;
				case sun3_instr_size_WORD:
					sun3_instr_subi_word.src= sun3_memory_read_unsigned_word(&vm->memory, pc);
					vm->pc += sizeof(sun3_unsigned_word);
					sun3_ea_mode_rw_word(vm, &sun3_instr_subi_word);
					break;
				default:
					sun3_errors_illegal_instruction(vm->errors);
				}
				break;
			case sun3_instr_ADDI:
				switch (size) {
				case sun3_instr_size_BYTE:
					sun3_instr_addi_byte.src= sun3_memory_read_unsigned_word(&vm->memory, pc);
					vm->pc += sizeof(sun3_unsigned_word);
					sun3_ea_mode_rw_byte(vm, &sun3_instr_addi_byte);
					break;
				case sun3_instr_size_LONG:
					sun3_instr_addi_long.src= sun3_memory_read_unsigned_long(&vm->memory, pc);
					vm->pc += sizeof(sun3_unsigned_long);
					sun3_ea_mode_rw_long(vm, &sun3_instr_addi_long);
					break;
				case sun3_instr_size_WORD:
					sun3_instr_addi_word.src= sun3_memory_read_unsigned_word(&vm->memory, pc);
					vm->pc += sizeof(sun3_unsigned_word);
					sun3_ea_mode_rw_word(vm, &sun3_instr_addi_word);
					break;
				default:
					sun3_errors_illegal_instruction(vm->errors);
				}
				break;
			case sun3_instr_EORI:
				switch (size) {
				case sun3_instr_size_BYTE:
					sun3_instr_eori_byte.src= sun3_memory_read_unsigned_word(&vm->memory, pc);
					vm->pc += sizeof(sun3_unsigned_word);
					sun3_ea_mode_rw_byte(vm, &sun3_instr_eori_byte);
					break;
				case sun3_instr_size_LONG:
					sun3_instr_eori_long.src= sun3_memory_read_unsigned_long(&vm->memory, pc);
					vm->pc += sizeof(sun3_unsigned_long);
					sun3_ea_mode_rw_long(vm, &sun3_instr_eori_long);
					break;
				case sun3_instr_size_WORD:
					sun3_instr_eori_word.src= sun3_memory_read_unsigned_word(&vm->memory, pc);
					vm->pc += sizeof(sun3_unsigned_word);
					sun3_ea_mode_rw_word(vm, &sun3_instr_eori_word);
					break;
				default:
					sun3_errors_illegal_instruction(vm->errors);
				}
				break;
			case sun3_instr_CMPI:
				switch (size) {
				case sun3_instr_size_BYTE:
					sun3_cmpi_byte(vm);
					break;
				case sun3_instr_size_LONG:
					sun3_cmpi_long(vm);
					break;
				case sun3_instr_size_WORD:
					sun3_cmpi_word(vm);
					break;
				default:
					sun3_errors_illegal_instruction(vm->errors);
				}
				break;
			default:
				sun3_errors_unimplemented_instruction(vm->errors);
			}
			break;
		case sun3_instr_MOVB:
			sun3_move_byte(vm);
			break;
		case sun3_instr_MOVL:
			sun3_move_long(vm);
			break;
		case sun3_instr_MOVW:
			sun3_move_word(vm);
			break;
		case 0x4:
			if (pci&(1<<8)) {
				switch (size) {
				case 0x2:		/* CHK */
					sun3_errors_unimplemented_instruction(vm->errors);
				case 0x3:		/* LEA */
				{
					sun3_memory_address addr= sun3_ea(vm);
					unsigned int addr_reg= sun3_instr_move_reg(pci);
					sun3_address_regs_write_unsigned_long(address_regs, addr_reg, addr);
				}
				break;
				default:
					sun3_errors_illegal_instruction(vm->errors);
				}
			} else {
				switch (sun3_instr_subcode(pci)) {
				case sun3_instr_CLR:
					switch (size) {
					case sun3_instr_size_BYTE:
						sun3_ea_mode_rw_byte(vm, &sun3_instr_clr_byte);
						break;
					case sun3_instr_size_LONG:
						sun3_ea_mode_rw_long(vm, &sun3_instr_clr_long);
						break;
					case sun3_instr_size_WORD:
						sun3_ea_mode_rw_word(vm, &sun3_instr_clr_word);
						break;
					default:
						sun3_errors_illegal_instruction(vm->errors);
					}
					break;
				case sun3_instr_NEG:
					switch (size) {
					case sun3_instr_size_BYTE:
						sun3_ea_mode_rw_byte(vm, &sun3_instr_neg_byte);
						break;
					case sun3_instr_size_LONG:
						sun3_ea_mode_rw_long(vm, &sun3_instr_neg_long);
						break;
					case sun3_instr_size_WORD:
						sun3_ea_mode_rw_word(vm, &sun3_instr_neg_word);
						break;
					default:
						sun3_errors_illegal_instruction(vm->errors);
					}
					break;
				case sun3_instr_NOT:
					switch (size) {
					case sun3_instr_size_BYTE:
						sun3_ea_mode_rw_byte(vm, &sun3_instr_not_byte);
						break;
					case sun3_instr_size_LONG:
						sun3_ea_mode_rw_long(vm, &sun3_instr_not_long);
						break;
					case sun3_instr_size_WORD:
						sun3_ea_mode_rw_word(vm, &sun3_instr_not_word);
						break;
					default:
						sun3_errors_illegal_instruction(vm->errors);
					}
					break;
				case sun3_instr_TST:
					switch (size) {
					case sun3_instr_size_BYTE:
						sun3_tst_byte(vm);
						break;
					case sun3_instr_size_LONG:
						sun3_tst_long(vm);
						break;
					case sun3_instr_size_WORD:
						sun3_tst_word(vm);
						break;
					default:
						sun3_errors_illegal_instruction(vm->errors);
					}
					break;
				case 0x8:
					switch (size) {
					case 0x0:		/* NBCD */
						sun3_errors_unimplemented_instruction(vm->errors);
					case 0x1:		/* SWAP, PEA */
						if (ea_mode == 0) {	/* SWAP */
							sun3_instr_swap(vm);
						} else {		/* PEA */
							sun3_memory_address addr= sun3_ea(vm);
							sun3_memory_address sp= sun3_address_regs_read_unsigned_long(address_regs, 7)-sizeof(sun3_memory_address);
							sun3_memory_write_long(mem, sp, addr);
							sun3_address_regs_write_unsigned_long(address_regs, 7, sp);
						}
						break;
					case 0x2:		/* EXT, MOVEM */
						if (ea_mode == sun3_instr_mode_DRD) { /* EXT.w */
							sun3_signed_word w= sun3_data_regs_read_signed_byte(data_regs, reg_num);
							sun3_data_regs_write_signed_word(data_regs, reg_num, w);
							sun3_sr_set_v(vm->sr, 0);
							sun3_sr_set_c(vm->sr, 0);
							sun3_sr_set_z(vm->sr, w==0);
							sun3_sr_set_n(vm->sr, w<0);
						} else {		/* MOVEM.w */
							sun3_errors_unimplemented_instruction(vm->errors);
						}
						break;
					case 0x3:
						if (ea_mode == sun3_instr_mode_DRD) { /* EXT.l */
							sun3_signed_long l= sun3_data_regs_read_signed_word(data_regs, reg_num);
							sun3_data_regs_write_signed_long(data_regs, reg_num, l);
							sun3_sr_set_v(vm->sr, 0);
							sun3_sr_set_c(vm->sr, 0);
							sun3_sr_set_z(vm->sr, l==0);
							sun3_sr_set_n(vm->sr, l<0);
						} else {		/* MOVEM.l */
							sun3_movem_reg_to_mem_long(vm);
						}
						break;
					default:
						assert(0);
					}
					break;
				case 0xc:
					switch (size) {
					case 0x2:
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x3:
						sun3_movem_mem_to_reg_long(vm);
						break;
					default:
						break;
					}
					break;
				case 0xe:
					switch (sun3_instr_4_7(pci)) {
					case sun3_instr_TRAP:
						sun3_instr_trap(vm);
						break;
					case 0x5:
						if (pci&(1<<3)) {
							sun3_errors_unimplemented_instruction(vm->errors);
						} else {
							sun3_errors_unimplemented_instruction(vm->errors);
						}
						break;
					case 0x7:
						switch (reg_num) {
						case sun3_instr_RESET:
							sun3_errors_privilige_violation(vm->errors);
						case sun3_instr_NOP:
							break;
						case sun3_instr_STOP:
							sun3_errors_privilige_violation(vm->errors);
						case sun3_instr_RTE:
							sun3_errors_privilige_violation(vm->errors);
						case sun3_instr_RTS:
						{
							sun3_memory_address sp = sun3_address_regs_read_unsigned_long(address_regs, 7);
							vm->pc = sun3_memory_read_unsigned_long(mem, sp);
							sp += sizeof(sun3_unsigned_long);
							sun3_trace_instr(vm, "rts");
							sun3_address_regs_write_unsigned_long(address_regs, 7, sp);
						}
						break;
						case sun3_instr_TRAPV:
							if (sun3_sr_v(vm->sr)) {
								sun3_errors_trap_exception(vm->errors);
							}
							break;
						case sun3_instr_RTR:
						{
							sun3_memory_address sp= sun3_address_regs_read_unsigned_long(address_regs, 7);
							sun3_unsigned_word ccr= sun3_memory_read_unsigned_word(mem, sp);
							vm->sr= (vm->sr&0xff)|(ccr&0xff);
							sp += sizeof(sun3_unsigned_word);
							vm->pc= sun3_memory_read_unsigned_long(mem, sp);
							sp += sizeof(sun3_unsigned_long);
							sun3_address_regs_write_unsigned_long(address_regs, 7, sp);
							break;
						}
						default:
							sun3_errors_illegal_instruction(vm->errors);
						}
						break;
					default:
						switch (size) {
						case 0x2:		/* JSR */
						{
							sun3_memory_address sp = sun3_address_regs_read_unsigned_long(&vm->address_regs, 7);
							sun3_memory_address npc = sun3_ea(vm);
							sp -= sizeof(sun3_unsigned_long);
							sun3_memory_write_long(&vm->memory, sp, vm->pc);
							sun3_address_regs_write_unsigned_long(&vm->address_regs, 7, sp);
							sun3_trace_instr(vm, "jsr %"sun3_memory_address_fmtx"", npc);
							vm->pc = npc;
							break;
						}
						case 0x3:		/* JMP */
							vm->pc = sun3_ea(vm);
							sun3_trace_instr(vm, "jmp %"sun3_memory_address_fmtx"", vm->pc);
							break;
						default:
							sun3_errors_illegal_instruction(vm->errors);
						}
						break;
					}
					break;
				default:
					sun3_errors_unimplemented_instruction(vm->errors);
				}
			}
			break;
		case 0x5:
			switch (size) {
			case sun3_instr_size_BYTE:
				sun3_instr_addq_or_subq_byte(vm);
				break;
			case sun3_instr_size_WORD:
				sun3_instr_addq_or_subq_word(vm);
				break;
			case sun3_instr_size_LONG:
				sun3_instr_addq_or_subq_long(vm);
				break;
			case 0x3:
				if (ea_mode == sun3_instr_mode_ARD) {
					sun3_instr_dbcc(vm);
				} else {
					sun3_scc(vm);
				}
				break;
			default:
				assert(0);
			}
			break;
		case 0x6:
			sun3_bcc_or_bsr(vm);
			break;
		case 0x7:			/* MOVEQ */
		{
			sun3_signed_long   q= (sun3_signed_byte)pci;
			sun3_data_regs_write_signed_long(data_regs, sun3_instr_move_reg(pci), q);
			sun3_sr_set_n(vm->sr, q<0);
			sun3_sr_set_z(vm->sr, q==0);
			sun3_sr_set_c(vm->sr, 0);
			sun3_sr_set_v(vm->sr, 0);
		}
		break;
		case 0x8:
			if ((pci&0x30) == 0) {		/* SBCD */
				sun3_errors_unimplemented_instruction(vm->errors);
			} else {
				switch (size) {
				case sun3_instr_size_BYTE:
					if (pci&(1<<8)) {
						sun3_or_ea_byte(vm);
					} else {
						sun3_or_reg_byte(vm);
					}
					break;
				case sun3_instr_size_WORD:
					if (pci&(1<<8)) {
						sun3_or_ea_word(vm);
					} else {
						sun3_or_reg_word(vm);
					}
					break;
				case sun3_instr_size_LONG:
					if (pci&(1<<8)) {
						sun3_or_ea_long(vm);
					} else {
						sun3_or_reg_long(vm);
					}
					break;
				case 0x3:		/* DIVS/DIVU */
					sun3_errors_unimplemented_instruction(vm->errors);
					break;
				default:
					assert(0);
					break;
				}
			}
			break;
		case 0x9:			/* SUB, SUBA, SUBX */
			if (pci&(1<<8)) {
				switch (size) {
				case sun3_instr_size_BYTE:
					switch (ea_mode) {
					case 0x0:		/* SUBX.b Dn */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x1:		/* SUBX.b -(An) */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					default:
						sun3_sub_ea_byte(vm);
						break;
					}
					break;
				case sun3_instr_size_LONG:
					switch (ea_mode) {
					case 0x0:		/* SUBX.l Dn */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x1:		/* SUBX.l -(An) */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					default:
						sun3_sub_ea_long(vm);
						break;
					}
					break;
				case sun3_instr_size_WORD:
					switch (ea_mode) {
					case 0x0:		/* SUBX.w Dn */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x1:
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					default:
						sun3_sub_ea_word(vm);
						break;
					}
					break;
				case 0x3:
					sun3_suba_long(vm);
					break;
				}
			} else {
				switch (size) {
				case sun3_instr_size_BYTE:
					sun3_sub_reg_byte(vm);
					break;
				case sun3_instr_size_LONG:
					sun3_sub_reg_long(vm);
					break;
				case sun3_instr_size_WORD:
					sun3_sub_reg_word(vm);
					break;
				case 0x3:
					sun3_suba_word(vm);
					break;
				}
			}
			break;
		case 0xb:			/* CMP, CMPA, CMPM */
			switch (size) {
			case sun3_instr_size_BYTE:
				if (pci&(1<<8)) {
					if (ea_mode == 0x01) { /* CMPM.b */
						sun3_errors_unimplemented_instruction(vm->errors);
					} else {
						sun3_instr_eor_byte.src= sun3_data_regs_read_unsigned_byte(&vm->data_regs, sun3_instr_move_reg(pci));
						sun3_ea_mode_rw_byte(vm, &sun3_instr_eor_byte);
					}
				} else {
					sun3_cmp_byte(vm);
				}
				break;
			case sun3_instr_size_LONG:
				if (pci&(1<<8)) {
					if (ea_mode == 0x01) { /* CMPM.l */
						sun3_errors_unimplemented_instruction(vm->errors);
					} else {
						sun3_instr_eor_long.src= sun3_data_regs_read_unsigned_long(&vm->data_regs, sun3_instr_move_reg(pci));
						sun3_ea_mode_rw_long(vm, &sun3_instr_eor_long);
					}
				} else {
					sun3_cmp_long(vm);
				}
				break;
			case sun3_instr_size_WORD:
				if (pci&(1<<8)) {
					if (ea_mode == 0x01) { /* CMPM.w */
						sun3_errors_unimplemented_instruction(vm->errors);
					} else {
						sun3_instr_eor_word.src= sun3_data_regs_read_unsigned_word(&vm->data_regs, sun3_instr_move_reg(pci));
						sun3_ea_mode_rw_word(vm, &sun3_instr_eor_word);
					}
				} else {
					sun3_cmp_word(vm);
				}
				break;
			case 0x3:
				if (pci&(1<<8)) {
					sun3_cmpa_long(vm);
				} else {
					sun3_cmpa_word(vm);
				}
				break;
			}
			break;
		case 0xc:			/* AND, EXG, MULS, MULU, ABCD */
			if (size == 0x3) {	/* MULS/MULU */
				if (pci&(1<<8)) {
					sun3_instr_muls(vm);
				} else {
					sun3_instr_mulu(vm);
				}
			} else if (((pci>>4)&0x3) != 0) { /* AND */
				switch (size) {
				case sun3_instr_size_BYTE:
					if (pci&(1<<8)) {
						sun3_and_ea_byte(vm);
					} else {
						sun3_and_reg_byte(vm);
					}
					break;
				case sun3_instr_size_LONG:
					if (pci&(1<<8)) {
						sun3_and_ea_long(vm);
					} else {
						sun3_and_reg_long(vm);
					}
					break;
				case sun3_instr_size_WORD:
					if (pci&(1<<8)) {
						sun3_and_ea_word(vm);
					} else {
						sun3_and_reg_word(vm);
					}
					break;
				default:
					assert(0);
				}
			} else if (size == 0x0) {	/* ABCD */
				sun3_errors_unimplemented_instruction(vm->errors);
			} else {			/* EXG */
				sun3_errors_unimplemented_instruction(vm->errors);
			}
			break;
		case 0xd:			/* ADD, ADDA, ADDX */
			if (pci&(1<<8)) {
				switch (size) {
				case sun3_instr_size_BYTE:
					switch (ea_mode) {
					case 0x0:		/* ADDX.b Dn */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x1:		/* ADDX.b -(An) */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					default:
						sun3_add_ea_byte(vm);
						break;
					}
					break;
				case sun3_instr_size_LONG:
					switch (ea_mode) {
					case 0x0:		/* ADDX.l Dn */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x1:		/* ADDX.l -(An) */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					default:
						sun3_add_ea_long(vm);
						break;
					}
					break;
				case sun3_instr_size_WORD:
					switch (ea_mode) {
					case 0x0:		/* ADDX.w Dn */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x1:		/* ADDX.w -(An) */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					default:
						sun3_add_ea_word(vm);
						break;
					}
					break;
				case 0x3:
					sun3_adda_long(vm);
					break;
				}
			} else {
				switch (size) {
				case sun3_instr_size_BYTE:
					sun3_add_reg_byte(vm);
					break;
				case sun3_instr_size_LONG:
					sun3_add_reg_long(vm);
					break;
				case sun3_instr_size_WORD:
					sun3_add_reg_word(vm);
					break;
				case 0x3:
					sun3_adda_word(vm);
				}
				break;
			}
			break;
		case 0xe:			/* AS[RL], LS[RL], ROX[RL], RO[RL] */
			if (pci&(1<<8)) {		/* ASL, LSL, ROXL, ROL */
				switch (size) {
				case sun3_instr_size_BYTE:
					switch ((pci>>3)&0x3) {
					case 0x0:		/* ASL.b */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x1:		/* LSL.b */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x2:		/* ROXL.b */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x3:		/* ROL.b */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					default:
						assert(0);
					}
					break;
				case sun3_instr_size_LONG:
					switch ((pci>>3)&0x3) {
					case 0x0:		/* ASL.l */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x1:		/* LSL.l */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x2:		/* ROXL.l */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x3:		/* ROL.l */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					default:
						assert(0);
					}
					break;
				case sun3_instr_size_WORD:
					switch ((pci>>3)&0x3) {
					case 0x0:		/* ASL.w */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x1:		/* LSL.w */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x2:		/* ROXL.w */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x3:		/* ROL.w */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					default:
						assert(0);
					}
					break;
				case 0x3:
					switch ((pci>>9)&0x3) {
					case 0x0:		/* ASL */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x1:		/* LSL */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x2:		/* ROXL */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x3:		/* ROL */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					default:
						assert(0);
					}
					break;
				}
			} else {			/* ASR, LSR, ROXR, ROR */
				switch (size) {
				case sun3_instr_size_BYTE:
					switch ((pci>>3)&0x3) {
					case 0x0:		/* ASR.b */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x1:		/* LSR.b */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x2:		/* ROXR.b */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x3:		/* ROR.b */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					default:
						assert(0);
					}
					break;
				case sun3_instr_size_LONG:
					switch ((pci>>3)&0x3) {
					case 0x0:		/* ASR.l */
					{
						unsigned int shift_count;
						unsigned int dest_reg= sun3_instr_reg(pci);
						sun3_unsigned_long dest;
						if (pci&(1<<5)) {
							shift_count= sun3_data_regs_read_unsigned_long(data_regs, sun3_instr_move_reg(pci))%64;
						} else {
							if ((shift_count= sun3_instr_move_reg(pci)) == 0)
								shift_count= 8;
						}
						dest= sun3_data_regs_read_unsigned_long(data_regs, dest_reg);
						if (sun3_unsigned_long_sign(dest)) {
							dest>>=shift_count;
						} else {
							dest>>=shift_count;
						}
						sun3_data_regs_write_unsigned_long(data_regs, dest_reg, dest);
					}
					break;
					case 0x1:		/* LSR.l */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x2:		/* ROXR.l */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x3:		/* ROR.l */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					default:
						assert(0);
					}
					break;
				case sun3_instr_size_WORD:
					switch ((pci>>3)&0x3) {
					case 0x0:		/* ASR.w */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x1:		/* LSR.w */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x2:		/* ROXR.w */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x3:		/* ROR.w */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					default:
						assert(0);
					}
					break;
				case 0x3:
					switch ((pci>>9)&0x3) {
					case 0x0:		/* ASR */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x1:		/* LSR */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x2:		/* ROXR */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					case 0x3:		/* ROR */
						sun3_errors_unimplemented_instruction(vm->errors);
						break;
					default:
						assert(0);
					}
					break;
				}
			}
			break;
		default:
			sun3_errors_unimplemented_instruction(vm->errors);
		}
	}
	return 1;
}
