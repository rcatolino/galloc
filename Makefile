CC=gcc #Compiler
EDL=gcc #Linker
AR=ar #Archiver
CCFLAGS=-Wall #Compiler options
EDLFLAGS=-Wall #Linker options
EXE=test #Binary name
LIB=gmem.a #Static library name
DEFINES=NODEBUG #Preprocessor definitions
ECHO=@echo

EOBJ=test.o
LOBJ=galloc.o gfree.o utils.o



$(EXE): $(EOBJ) $(LIB)
	@echo building $<
	$(EDL)  -o $(EXE) $(EDLFLAGS) $(EOBJ) $(LIB)
	@echo done

$(LIB): $(LOBJ)
	@echo building $<
	$(AR)  rcs $(LIB) $(LOBJ)
	@echo done

%.o : %.c *.h
	@echo building $< ...
	$(CC) $(CCFLAGS) -c -D $(DEFINES) $<
	@echo done
	
clean: 
	@echo -n cleaning repository... 
	@rm -rf *.o
	@echo cleaned.
