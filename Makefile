CC=gcc #Compiler
EDL=gcc #Linker
AR=ar #Archiver
CCFLAGS=-Wall -m32 -fpic #Compiler options
EDLFLAGS=-Wall -m32 #Linker options
EXE=test #Binary name
LIB=libgmem.so#Static library name
DEFINES=NODEBUG #Preprocessor definitions
ECHO=@echo

EOBJ=test.o 
LOBJ=galloc.o gfree.o utils.o

lib : $(LIB)

install : $(LIB)
	@cp ./libgmem.so.1.0.1 /usr/lib32/
	@cp gmem.h /usr/include
	@echo Updating ld...
	ldconfig -n /usr/lib32/

$(EXE): $(EOBJ)
	@echo building $<
	$(EDL)  -o $(EXE) $(EDLFLAGS) $(EOBJ) -lgmem
	@echo done

$(LIB): $(LOBJ)
	@echo building $<
	$(CC) -m32 -shared -Wl,-soname,$(LIB) -o $(LIB).1.0.1 $(LOBJ)
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
