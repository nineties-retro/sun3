#ifndef SUN3_INSTRUCTION_H
#define SUN3_INSTRUCTION_H

#include "sun3_machine_types.h"
#include "sun3_sr.h"

enum sun3_ea_mode {
	sun3_ea_mode_drd=   (1<<1),
	sun3_ea_mode_ard=   (1<<2),
	sun3_ea_mode_ari=   (1<<3),
	sun3_ea_mode_aripi= (1<<4),
	sun3_ea_mode_aripd= (1<<5),
	sun3_ea_mode_arid=  (1<<6),
	sun3_ea_mode_arii=  (1<<7),
	sun3_ea_mode_asa=   (1<<8),
	sun3_ea_mode_ala=   (1<<9),
	sun3_ea_mode_pcd=   (1<<10),
	sun3_ea_mode_pci=   (1<<11),
	sun3_ea_mode_id=    (1<<12),
	sun3_ea_mode_sr=    (1<<13),
};

typedef enum sun3_ea_mode sun3_ea_mode;

typedef struct sun3_byte_instruction sun3_byte_instruction;

typedef void (*sun3_byte_instruction_action)(sun3_byte_instruction *);
typedef void (*sun3_byte_instruction_flags)(sun3_byte_instruction *, sun3_sr *);

struct sun3_byte_instruction {
	sun3_unsigned_byte            src;
	sun3_unsigned_byte            dest;
	sun3_unsigned_byte            ans;
	sun3_ea_mode                  legal_execute_modes;
	sun3_byte_instruction_action  execute;
	sun3_ea_mode                  legal_flag_modes;
	sun3_byte_instruction_flags   set_flags;
};



typedef struct sun3_word_instruction sun3_word_instruction;

typedef void (*sun3_word_instruction_action)(sun3_word_instruction *);
typedef void (*sun3_word_instruction_flags)(sun3_word_instruction *, sun3_sr *);

struct sun3_word_instruction {
	sun3_unsigned_word            src;
	sun3_unsigned_word            dest;
	sun3_unsigned_word            ans;
	sun3_ea_mode                  legal_execute_modes;
	sun3_word_instruction_action  execute;
	sun3_ea_mode                  legal_flag_modes;
	sun3_word_instruction_flags   set_flags;
};



typedef struct sun3_long_instruction sun3_long_instruction;

typedef void (*sun3_long_instruction_action)(sun3_long_instruction *);
typedef void (*sun3_long_instruction_flags)(sun3_long_instruction *, sun3_sr *);

struct sun3_long_instruction {
	sun3_unsigned_long            src;
	sun3_unsigned_long            dest;
	sun3_unsigned_long            ans;
	sun3_ea_mode                  legal_execute_modes;
	sun3_long_instruction_action  execute;
	sun3_ea_mode                  legal_flag_modes;
	sun3_long_instruction_flags   set_flags;
};

#endif
