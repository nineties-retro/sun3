#ifndef SUN3_INSTR_H
#define SUN3_INSTR_H

#define sun3_instrs(n) ((n)*2)

#define sun3_instr_opcode(instr) ((instr)>>12)
#define sun3_instr_subcode(instr) (((instr)>>8)&0xf)
#define sun3_instr_4_7(instr) (((instr)>>4)&0xf)
#define sun3_instr_6_8(instr) (((instr)>>6)&0x7)
#define sun3_instr_9_11(instr) (((instr)>>9)&0x7)
#define sun3_instr_move_mode(instr) sun3_instr_6_8(instr)
#define sun3_instr_move_ea_or_reg(instr) sun3_instr_9_11(instr)
#define sun3_instr_move_reg(instr) sun3_instr_9_11(instr)
#define sun3_instr_qdata(instr) sun3_instr_9_11(instr)
#define sun3_instr_trap_addr(instr) ((instr)&0x000f)
#define sun3_instr_8bit(instr) ((instr)&0x000f)

#define sun3_instr_ea_mode(instr) (((instr)>>3)&0x7)
#define sun3_instr_mode_DRD   0x0 /* Data Register Direct */
#define sun3_instr_mode_ARD   0x1 /* Address Register Direct */
#define sun3_instr_mode_ARI   0x2 /* Address Register Indirect */
#define sun3_instr_mode_ARIPI 0x3 /* Address Register Indirect with ++ */
#define sun3_instr_mode_ARIPD 0x4 /* Address Register Indirect with -- */
#define sun3_instr_mode_ARID  0x5 /* Adresss Register Indirect with Displacement */
#define sun3_instr_mode_ARII  0x6 /* Address Register Indirect with Index */
#define sun3_instr_mode_EXTD  0x7 /* Extended mode ... */
#define sun3_instr_mode_ASA   0x0 /* Absolute Short Address */
#define sun3_instr_mode_ALA   0x1 /* Absolute Long Address */
#define sun3_instr_mode_PCD   0x2 /* Program Counter with Displacement */
#define sun3_instr_mode_PCI   0x3 /* Program Counter with Index */
#define sun3_instr_mode_ID    0x4 /* Immediate Data */
#define sun3_instr_mode_SRCR  0x4 /* Status Reg or Condition Reg */

#define sun3_instr_reg(instr) ((instr)&0x7)

#define sun3_instr_size(instr) (((instr)>>6)&0x3)
#define sun3_instr_size_BYTE 0x0
#define sun3_instr_size_WORD 0x1
#define sun3_instr_size_LONG 0x2

#define sun3_instr_condition(instr) (((instr)>>8)&0xf)

#define sun3_instr_MOVB 0x1
#define sun3_instr_MOVL 0x2
#define sun3_instr_MOVW 0x3

#define sun3_instr_CLR  0x2
#define sun3_instr_NEG  0x4
#define sun3_instr_NOT  0x6
#define sun3_instr_TST  0xa

#define sun3_instr_TRAP 0x4

#define sun3_instr_RESET 0x0
#define sun3_instr_NOP   0x1
#define sun3_instr_STOP  0x2
#define sun3_instr_RTE   0x3
#define sun3_instr_RTS   0x5
#define sun3_instr_TRAPV 0x6
#define sun3_instr_RTR   0x7

#define sun3_instr_ORI  0x0
#define sun3_instr_ANDI 0x2
#define sun3_instr_SUBI 0x4
#define sun3_instr_ADDI 0x6
#define sun3_instr_EORI 0xa
#define sun3_instr_CMPI 0xc

#define sun3_condition_T  0x0
#define sun3_condition_F  0x1
#define sun3_condition_HI 0x2
#define sun3_condition_LS 0x3
#define sun3_condition_CC 0x4
#define sun3_condition_HS 0x4
#define sun3_condition_CS 0x5
#define sun3_condition_LO 0x5
#define sun3_condition_NE 0x6
#define sun3_condition_EQ 0x7
#define sun3_condition_VC 0x8
#define sun3_condition_VS 0x9
#define sun3_condition_PL 0xa
#define sun3_condition_MI 0xb
#define sun3_condition_GE 0xc
#define sun3_condition_LT 0xd
#define sun3_condition_GT 0xe
#define sun3_condition_LE 0xf

#endif
