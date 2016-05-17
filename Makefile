FLAGS=-g -std=c++11 -fPIC
CXX=g++
LDFLAGS=-ldl
SHARED=-shared

SOURCES=	package.cpp \
			container.cpp \
			graph.cpp \
			cyclesearcher.cpp \
			cyclecontainer.cpp \
			utils.cpp \
			graphcrunch.cpp

EXPDEPS=	package.cpp \
			graph.cpp \
			cyclecontainer.cpp \
			cyclesearcher.cpp \
			utils.cpp \
			container.cpp 

PARSEDEPS=	package.cpp \
			container.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXPOBJ=$(EXPDEPS:.cpp=.o)
PARSEOBJ=$(PARSEDEPS:.cpp=.o)

BINARIES=graphcrunch

.PHONY: all		\
		shared 	\
		clean

all: $(OBJECTS) shared
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(BINARIES)

shared: dotexport.so fedparser.so testexport.so

dotexport.so: $(EXPOBJ)
	$(CXX) $(SHARED) $(FLAGS) $(EXPOBJ) dotexport.cpp -o dotexport.so

testexport.so: $(EXPOBJ)
	$(CXX) $(SHARED) $(FLAGS) $(EXPOBJ) testexport.cpp -o testexport.so

fedparser.so: $(PARSEOBJ)
	$(CXX) $(SHARED) $(FLAGS) $(PARSEOBJ) fedparser.cpp -o fedparser.so

.cpp.o:
	$(CXX) $(FLAGS) -c $<

.cpp.so:
	$(CXX) $(FLAGS) -c $<

clean:
	rm -f $(BINARIES)
	rm -f *.o	
	rm -f *.so