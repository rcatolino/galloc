CC=gcc #Compiler
EDL=gcc #Linker
AR=ar #Archiver
CCFLAGS=-Wall -fpic #Compiler options
EDLFLAGS=-Wall #Linker options
EXE=test #Binary name
LIB=libgmem.so#Static library name
DEFINES=NODEBUG #Preprocessor definitions
ECHO=@echo

EOBJ=test.o 
LOBJ=galloc.o gfree.o utils.o

lib : $(LIB)

install : $(LIB)
	@cp ./libgmem.so.1.0.1 /usr/lib/
	@cp gmem.h /usr/include
	ldconfig -n -v /usr/lib/

$(EXE): $(EOBJ)
	@echo building $<
	$(EDL)  -o $(EXE) $(EDLFLAGS) $(EOBJ) -lgmem
	@echo done

$(LIB): $(LOBJ)
	@echo building $<
	$(CC) -shared -Wl,-soname,$(LIB) -o $(LIB).1.0.1 $(LOBJ)
	@echo done

%.o : %.c *.h
	@echo building $< ...
	$(CC) $(CCFLAGS) -c -D $(DEFINES) $<
	@echo done
	
clean: 
	@echo -n cleaning repository... 
	@rm -rf *.o
	@rm -rf *.so*
	@echo cleaned.
