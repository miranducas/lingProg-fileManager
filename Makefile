################# Makefile #################
#### flags
PERL=perl

CPP=g++

CPPFLAGS=-Wall -std=c++11 $(shell perl -MExtUtils::Embed -e ccopts -e ldopts)

LD=g++

LFLAGS=$(shell perl -MExtUtils::Embed -e ldopts)

#targets
MAINCFILE=  main.cpp\
			Classes/Exclusao.cpp\
			Classes/Pesquisa.cpp
MAINOBJS= main.o\
		  #Classes/Exclusao.o\
		  #Classes/Pesquisa.o 

MAINEXECS=main

TEXTFILES = parametros.txt registro.txt lista-de-retorno.txt

#.cpp.o:
#	$(CPP) $(CPPFLAGS) -c $(MAINCFILE) -o $(MAINOBJS) 

all: $(MAINEXECS)

main:$(MAINOBJS)
	$(CPP) $(CPPFLAGS) -c $(MAINCFILE)
	$(LD) -L/usr/lib -o $(MAINEXECS) $(MAINOBJS) $(LFLAGS)
#	$(LD) -o $@ $(MAINOBJS) $(LFLAGS)


clean:
	rm -rf *.o
	rm -rf main
	rm -rf $(TEXTFILES)