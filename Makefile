FLAGS=-g -std=c++11
CXX=g++
LDFLAGS=
SOURCES=package.cpp \
		container.cpp \
		parser.cpp \
		graph.cpp \
		cyclesearcher.cpp \
		cyclecontainer.cpp \
		dotexport.cpp \
		graphdump.cpp

OBJECTS=$(SOURCES:.cpp=.o)
BINARIES=graphdump

all: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(BINARIES)

.cpp.o:
	$(CXX) $(FLAGS) -c $<

clean:
	rm -f $(BINARIES)
	rm -f *.o	