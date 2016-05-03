FLAGS=-g -std=c++11 -fPIC
CXX=g++
LDFLAGS=-ldl
SHARED=-shared

SOURCES=	package.cpp \
			container.cpp \
			parser.cpp \
			graph.cpp \
			cyclesearcher.cpp \
			cyclecontainer.cpp \
			exporterfactory.cpp \
			graphdump.cpp

EXPDEPS=	package.cpp \
			graph.cpp \
			cyclecontainer.cpp \
			cyclesearcher.cpp \
			container.cpp 

OBJECTS=$(SOURCES:.cpp=.o)
EXPOBJ=$(EXPDEPS:.cpp=.o)

BINARIES=graphdump

.PHONY: all		\
		shared 	\
		clean

all: $(OBJECTS) shared
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(BINARIES)

shared: dotexport.so

dotexport.so: $(EXPOBJ)
	$(CXX) $(SHARED) $(FLAGS) $(EXPOBJ) dotexport.cpp -o dotexport.so

.cpp.o:
	$(CXX) $(FLAGS) -c $<

.cpp.so:
	$(CXX) $(FLAGS) -c $<

clean:
	rm -f $(BINARIES)
	rm -f *.o	