/* Contains major function that are related to the first pass */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Errors.h"
#include "globals.h"
#include "table.h"
#include "util.h"
#include "first_pass.h"


int handle_first_pass(line_data current_line_struct, int *IC, int *DC, table symbol_table){
    int char_index;
    char current_symbol[MAX_LABEL_LENGTH];

    /*Initiate the char index to 0 */
    char_index = 0;

    /*!This case will be handle by Tamir in the pre-process*/
    /*Skip over not relevant chars*/
    SKIP_WHITE_SPACE(current_line_struct.data,char_index);





    return SUCCESS;
}

