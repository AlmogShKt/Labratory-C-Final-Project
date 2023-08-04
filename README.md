# Labratory-C-Final-Project
This is the Final project (mmn 14) in Laboratory C 20465- 2023B course at the Open University

This project was developed as part of the "20465 - Systems Programming Laboratory" 2023B course at the Open University. It's a comprehensive assembler program for a specific assembly language, written in C. The purpose is to simulate the operation of common system programs, and thus experience writing a large-scale software project.
___
## Project Structure

The project consists of multiple C and header files, each serving a specific purpose in the program. It adheres to principles of clarity, readability, structured writing, and data abstraction.

### The assembler works in three main stages:

1. **Pre-Processing**
2. **First Pass**
3. **Second Pass**


These stages form the core algorithm of the assembler.

### Pre-Processing

The pre-assembler reads the source file line by line and identifies macro definitions. If a macro is identified, the lines of the macro are added to a macro table and replaced in the source file. Here's the basic algorithm:

1. Read line from source file. If the file is finished, end this phase.
2. If the first field is a macro name, replace it in the source file with appropriate lines from the macro table. Repeat step 1.
3. If the first field is "mcro" (indicating the start of a macro definition), flag it and proceed to the next steps.
4. For each line while the macro flag is on, add the line to the macro table and remove it from the source file. If an "endmcro" label is detected, remove the flag and return to step 1. Otherwise, continue to the next line.

### First Pass

The first pass identifies symbols (labels) and assigns them numerical values representing their corresponding memory addresses. Here's the basic process:

1. Initialize Data Counter (DC) and Instruction Counter (IC) to 0.
2. Read line from source file. If the file is finished, proceed to the next stage.
3. If the first field is a symbol, flag it.
4. If the line is a data storage directive (.data or .string), and if a symbol has been flagged, add it to the symbol table with its value set to the current DC. Then, encode the data in memory and update DC accordingly.
5. If the line is an .extern or .entry directive, and if it's an .extern directive, add any symbol used as an operand to the symbol table without a value and marked as external.
6. If a symbol is flagged and it's not a directive, add it to the symbol table with its value set to the current IC.
7. Find the instruction name in the instruction table and report an error if not found.
8. Analyze the instruction's operands and calculate the length (L). Construct the binary code for the first word of the instruction and update IC.
9. If any errors were found in the source file, stop.
10. Update the values of the data type symbols in the symbol table by adding the final value of IC.

### Second Pass

In the second pass, the assembler generates the final machine code, replacing operation names with their binary equivalents and symbol names with their assigned memory locations. Details about the algorithm for the second pass will be added later.
 ___ 
## Getting Started

### Prerequisites

- GCC compiler
- Ubuntu system (but also other will work)

### Installation

1. Clone the repository: `~git clone <repo_link>`
2. Navigate to the cloned repository: `~cd <repo_name>`
3. Run Makefile to compile the program: `~make`

## Usage

Run the compiled program using the following command:`~./assembler file_name_1 ... file_name_N`

This will output machine code generated from the provided assembly file.

This is valid input  and output files:  
<img src="Readme_imgs/c_master_as.png" width="200"> <img src="Readme_imgs/valid_output1.png" width="200">    


And for Invalid output:
<img src="Readme_imgs/Invalid1.png" width="200">
___
### Testing

Several assembly language input files are provided to demonstrate the use of various operations and data types, as well as the assembler's handling of errors.
___
## Documentation

Each function in the source code is accompanied by concise, clear documentation in the form of header comments, explaining its purpose and usage. Important variables are also explained. Detailed comments are present throughout the code for better understanding.
___
## Contribution

This project does not accept contributions as of now.


