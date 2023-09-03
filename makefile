# Compilation macros
 CC = gcc
 CFLAGS = -ansi -Wall -pedantic -g # Flags
 GLOBAL_DEPS = globals.h # Dependencies for everything
 EXE_DEPS = assembler.o  util.o table.o preproc.o first_pass.o second_pass.o code_conversion.o data_strct.o Errors.o handle_text.o lexer.o # Deps for exe

 ## Executable
assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

assembler.o:  ../SourceFiles $(GLOBAL_DEPS)
	$(CC) -c assembler.c $(CFLAGS) -o $@

preproc.o: ../SourceFiles Header $(GLOBAL_DEPS)
	$(CC) -c preproc.c $(CFLAGS) -o $@

first_pass.o: ../SourceFiles Header $(GLOBAL_DEPS)
	$(CC) -c first_pass.c $(CFLAGS) -o $@

second_pass.o: ../SourceFiles Header $(GLOBAL_DEPS)
	$(CC) -c second_pass.c $(CFLAGS) -o $@

code_conversion.o: ../SourceFiles Header $(GLOBAL_DEPS)
	$(CC) -c code_conversion.c $(CFLAGS) -o $@

data_strct.o: ../SourceFiles Header $(GLOBAL_DEPS)
	$(CC) -c data_strct.c $(CFLAGS) -o $@

table.o: ../SourceFiles Header $(GLOBAL_DEPS)
	$(CC) -c table.c $(CFLAGS) -o $@

util.o: ../SourceFiles Header $(GLOBAL_DEPS)
	$(CC) -c util.c $(CFLAGS) -o $@

Errors.o: ../SourceFiles Header $(GLOBAL_DEPS)
	$(CC) -c Errors.c $(CFLAGS) -o $@

handle_text.o: ../SourceFiles Header $(GLOBAL_DEPS)
	$(CC) -c handle_text.c $(CFLAGS) -o $@

lexer.o: ../SourceFiles Header $(GLOBAL_DEPS)
	$(CC) -c lexer.c $(CFLAGS) -o $@

clean:
	rm -rf *.o *.am *.ob *.ent *.ext
