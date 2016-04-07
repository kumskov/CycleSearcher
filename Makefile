FLAGS=-g -std=c++11
CXX=g++

BINARIES=	graphdump

all: $(BINARIES)

#graphdump: package.o container.o parser.o graph.o cyclesearcher.o graphdump.o
#	$(CXX) $(FLAGS) package.o parser.o graph.o container.o cyclesearcher.o graphdump.o -o graphdump

graphdump: package.o container.o parser.o graph.o graphdump.o
	$(CXX) $(FLAGS) package.o parser.o graph.o container.o graphdump.o -o graphdump

package.o: package.cpp package.hpp
	$(CXX) $(FLAGS) -c package.cpp

container.o: parser.cpp parser.hpp
	$(CXX) $(FLAGS) -c container.cpp

parser.o: parser.cpp parser.hpp
	$(CXX) $(FLAGS) -c parser.cpp

graph.o: graph.cpp graph.hpp
	$(CXX) ${FLAGS} -c graph.cpp

#cyclesearcher.o: cyclesearcher.cpp cyclesearcher.hpp
#	$(CXX) ${FLAGS} -c cyclesearcher.cpp

test.o: test.cpp
	$(CXX) $(FLAGS) -c test.cpp

graphdump.o: graphdump.cpp
	$(CXX) $(FLAGS) -c graphdump.cpp

clean:
	rm -f $(BINARIES)
	rm -f *.o	