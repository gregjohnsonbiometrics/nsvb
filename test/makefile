VPATH = ../src

CPPFLAGS = -c -std=c++23 -O3 -Wall -I"../src" 

SOURCES= test.cpp nsvb.cpp
OBJECTS=$(SOURCES:.cpp=.o)

# make test
test: $(OBJECTS)
	g++ -static $(OBJECTS) -o $@
	
# run the test
RUN: 
	./test
	
.cpp.o:
	g++ $(CPPFLAGS) $< -o $@

clean:
	rm *.o
