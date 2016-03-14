FLAGS=-g -std=c++11
CXX=g++

BINARIES=	test

all: $(BINARIES)

test: package.o container.o parser.o graph.o test.o
	$(CXX) $(FLAGS) package.o parser.o graph.o container.o test.o -o test

package.o: package.cpp package.hpp
	$(CXX) $(FLAGS) -c package.cpp

container.o: parser.cpp parser.hpp
	$(CXX) $(FLAGS) -c container.cpp

parser.o: parser.cpp parser.hpp
	$(CXX) $(FLAGS) -c parser.cpp

graph.o: graph.cpp graph.hpp
	$(CXX) ${FLAGS} -c graph.cpp

test.o: test.cpp
	$(CXX) $(FLAGS) -c test.cpp

clean:
	rm -f $(BINARIES)
	rm -f *.o	