################################

# SYSTEM   = x86-64_linux
SYSTEM     = x86-64_osx
LIBFORMAT  = static_pic
BASE_DIR   = /Applications/CPLEX_Studio221
CPLEXDIR   = ${BASE_DIR}/cplex
CONCERTDIR = ${BASE_DIR}/concert

################

# Compilador 
# CCC = g++ -O0
CCC = arch -x86_64 clang++ -std=c++20 -gfull

# Opcoes de compilacao
CCOPT = -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD -Wno-ignored-attributes

# Bibliotecas e includes
CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(SYSTEM)
CPLEXLIB      = cplex$(dynamic:yes=2010)

CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR) $(dynamic:yes=-L$(CPLEXBINDIR))
CCLNFLAGS = -lconcert -lilocplex -l$(CPLEXLIB) -lm -lpthread -ldl

CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) 

all : facilities_solver

facilities_solver: facilities_solver.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) facilities_solver.o -o facilities_solver $(CCLNFLAGS)
facilities_solver.o: facilities_solver.cpp
	$(CCC) -c $(CCFLAGS) facilities_solver.cpp -o facilities_solver.o

clean:
	rm --force ex1 ex1.o ex2 ex2.o ex3 ex3.o tsp tsp.o data.o facilities_solver facilities_solver.o
