#ifndef C_PROJECT_SECOND_PASS_H
#define C_PROJECT_SECOND_PASS_H

#include "table.h"
#include "code_conversion.h"


/**
 * Execute the second pass of the assembler.
 *
 * @brief This function executes the second pass of the assembler. It performs various tasks such as checking for errors, merging
 * code and data, replacing externs and labels, converting code to base64 format, and printing output files. The function
 * takes the output of the first pass, label_table, IC (Instruction Counter), DC (Data Counter), externs, and entries
 * as input. It performs several checks for errors during the second pass and sets the 'error_found' flag if any error is
 * encountered.
 *
 * @param file_name         The name of the source file being processed (used for creating output files).
 * @param label_table       A pointer to the label_address array containing information about defined labels.
 * @param IC                The Instruction Counter.
 * @param DC                The Data Counter.
 * @param label_table_line  The number of entries in the 'label_table' array.
 * @param externs_count     The number of entries in the 'externs' array.
 * @param entries_count     The number of entries in the 'entries' array.
 * @param code              A pointer to the code_conv array containing the assembled code.
 * @param data              A pointer to the code_conv array containing the assembled data.
 * @param externs           A pointer to the other_table array containing information about extern labels.
 * @param entries           A pointer to the other_table array containing information about entry labels.
 * @param error_found       A flag indicating if any error is found during the second pass (0 - no errors, 1 - errors found).
 * @return Returns 1 if the second pass is executed successfully without errors, 0 otherwise.
 */
int exe_second_pass(char *file_name, label_address *label_table, int IC, int DC, int label_table_line, \
    int externs_count, int entries_count, code_conv *code, code_conv *data, other_table *externs, other_table *entries, \
    int error_found);


/**
* Convert the binary code to base64 format and write it to the output file.
*
* This function takes the binary code in 'code' array, the total number of instructions and data in 'count',
* the name of the source file 'file_name', and the Instruction Counter (IC) and Data Counter (DC) as inputs. It
* converts each machine code in the 'code' array to its base64 representation and writes the result to the output
* file with a '.ob' extension. The first line of the output file contains the values of IC and DC, separated by a
* space. The subsequent lines contain the base64 representation of each machine code in the 'code' array.
*
* @param code        A pointer to the code_conv array containing the binary code that represent to assembler code.
* @param count       The total number of instructions and data in the 'code' array.
* @param file_name   The name of the source file being processed (used for creating output files).
* @param IC          The Instruction Counter.
* @param DC          The Data Counter.
* @return Returns 1 if the conversion and writing to the output file are successful, 0 otherwise.
*/
int conv_code_base64(code_conv *code, int count, char *file_name, int IC, int DC);


#endif
