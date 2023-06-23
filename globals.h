#ifndef LABRATORY_C_FINAL_PROJECT_GLOBALS_H
#define LABRATORY_C_FINAL_PROJECT_GLOBALS_H
/*The File contain all the global values in the program*/

/* Maximum length of a label in command line  */
#define MAX_LABEL_LENGTH 31

/* Maximum length of a single command line  */
#define MAX_LINE_LENGTH 80

/* Default IC value */
#define IC_INIT_VALUE 100

/* arbitrary very big number for line length */
#define SIZE 1000

#define OPCODES_COUNT 16

#define REG_COUNT 8

#define WORD_LEN 12

#define ARE_BITS 2

#define RELOCATABLE_VALUE 2

#define EXTERNAL_VALUE 1

/* Defines a better way to work with Ture / False values  - using enum*/
typedef enum booleans {
    TRUE = 1,
    FALSE = 0
} bool;

/* Defines the addressing methods for the operands and a number that represent the function type*/
typedef enum symbol_type {
    ENTRY_SYMBOL = 0,
    DATA_SYMBOL = 1,
    EXTERN_SYMBOL = 2,
    /*! Code?! maybe just a regular line? 'code' ?*/
    CODE_SYMBOL = 3
} symbol_type;


/* Commands opcode */
typedef enum opcodes_values {
    /*The format is the name of the command 'OPCODENAME_C', C stands for Code*/

    /*1st group*/
    MOV_C = 0,
    CMP_C = 1,
    ADD_C = 2,
    SUB_C = 3,
    LEA_C = 6,

    /*2nd group*/
    NOT_C = 4,
    CLR_C = 5,
    INC_C = 7,
    DEC_C = 8,
    JMP_C = 9,
    BNE_C = 10,
    RED_C = 11,
    PRN_C = 12,
    JSR_C = 13,

    /*3rd group*/
    RTS_C = 14,
    STOP_C = 15,

    /*Value for wrong Opcode name */
    WRONG_C = -1
} opcode;


/** Registers - r0->r1 + not found */
typedef enum registers {
    R0 = 0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    NONE_REG = -1
} reg;


typedef struct instruction_machine_word {
    /*ARE*/
    /* A - '00' - Absolut | R - Relocatable - '10' | E - '01' - External*/
    unsigned int ARE: 2;
    unsigned int destination_operand: 2;
    unsigned int opcode: 4;
    unsigned int source_operand: 2;

    /*In Total 12 bytes*/
} instruction_machine_word;

/* Defines a machine word of operand with direct or immediate addressing */
typedef struct immediate_direct_word {
    unsigned int ARE: 2;
    /*The 10 bytes store the value*/
    unsigned int operand: 10;
} immediate_direct_word;

/* Defines a machine word of registers operands(direct register addressing)*/
typedef struct register_word {
    unsigned int ARE: 2;
    unsigned int destination_register: 5;
    unsigned int source_register: 5;
} register_word;

/* Defines a general machine word - first word, immediate or direct addressing word,
    register word or data word */
typedef struct machine_word {
    char *label; /* We'll put here label names in the first pass */
    union word {
        instruction_machine_word *f_word;
        immediate_direct_word *id_word;
        register_word *r_word;
        /*! label word??!*/
    } word;
} machine_word;

/*Defines a general word for labels: .data / .string / .external .entry*/
typedef struct label_word {
    unsigned int data: 12;
} label_word;


/* Define a line struct*/
typedef struct line_data {
    /*The file name will help us to print relevant errors*/
    char *file_name;
    /*The line number will helps us to track the current line number and to print errors*/
    short number;
    /*The content of the line*/
    char *data;

} line_data;

/*Finals for file extensions names */
#define AS_FILE_EXT ".as"
#define TXT_FILE_EXT ".txt"
#define AM_FILE_EXT ".am"


#endif