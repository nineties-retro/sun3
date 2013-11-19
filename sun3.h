/* XH: sun3_errors.h */
/* XH: sun3_data_regs.h */
/* XH: sun3_address_regs.h */
/* XH: sun3_memory.h */

struct sun3_errors;

struct sun3 {
	char const         * image_file_name;
	sun3_memory           memory;
	sun3_memory_address   start_address;
	sun3_data_regs        data_regs;
	sun3_address_regs     address_regs;
	struct sun3_errors * errors;
	
	sun3_memory_address   pc;
	sun3_unsigned_word    pci;
	unsigned int          ea_mode;
	unsigned int          size;
	unsigned int          reg_num;
	sun3_sr               sr;
};

typedef struct sun3 sun3;
