#Variables utilisées :
CC=gcc #Compilateur
EDL=gcc #Linker
CCFLAGS=-Wall #Options de compilations
EDLFLAGS=-Wall
EXE=galloc #Nom du binaire à construire
ECHO=@echo

OBJ=galloc.o
LIBS=



$(EXE): $(OBJ) $(LIBS)
	@echo building $<
	$(EDL)  -o $(EXE) $(EDLFLAGS) $(OBJ) $(LIBS)
	@echo done

%.o : %.cpp *.h
	@echo building $< ...
	$(CC) $(CCFLAGS) -c $<
	@echo done
	
clean: 
	@echo -n cleaning repository... 
	@rm -rf *.o
	@echo cleaned.
