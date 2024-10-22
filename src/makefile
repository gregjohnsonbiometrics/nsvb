OS_NAME = $(shell uname -o)

# make test
test: test.cpp nsvb.cpp nsvb.hpp nsvb_coef.hpp
	g++ -std=c++23 -Wall -O3 test.cpp nsvb.cpp -o test

# make dynamic link library
dll: nsvb.cpp nsvb.hpp nsvb_coef.hpp
	g++ -c -std=c++23 -Wall -O3 nsvb.cpp
ifeq ($(OS_NAME),Linux)
	g++ -shared nsvb.o -o nsvb.a
else
	g++ -shared nsvb.o -o nsvb.dll
endif	
	
# run the test
RUN: 
	./test
	