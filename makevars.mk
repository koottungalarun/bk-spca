#########
CPP 	 := g++
CFLAGS 	 := -O3  -Drestrict=__restrict__ -Wshadow -MMD -MP

#########
LALIBS := -lblas -llapack
LATHRD := -lblas -llapack
FFTWLIB  := -lfftw3 

#########
.SUFFIXES:
.SUFFIXES: .cpp .o .exe .s .d
%.o: %.cpp
	$(CPP) $(EXTRNL) $(CFLAGS)  -c $<
%.s: %.cpp 
	$(CPP) $(EXTRNL) $(CFLAGS) -fno-verbose-asm  -S $< 
%.o: %.s 
	$(CPP) $(EXTRNL) $(CFLAGS) -c $< 
%.exe: %.o 
	$(CPP) $(EXTRNL) $(LFLAGS) -o $@ $(filter %.o,$^) $(LIBS) 

########
.PHONY: clean cleanx cleanxx
clean:
	rm -f *.o; rm -f *.exe; rm -f a.out;

cleanx:
	rm -f pbs*.*; 

cleanxx:
	rm -f *.o; rm -f *.exe; rm -f pbs*.*; rm -f *.d; rm -f DBG/outP*; \
	rm -f slurm*.out;
